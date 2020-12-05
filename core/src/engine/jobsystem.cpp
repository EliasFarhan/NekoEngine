/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <engine/jobsystem.h>

#include <utility>

#ifdef EASY_PROFILE_USE
#include <easy/profiler.h>
#endif

namespace neko
{

JobSystem::JobSystem()
{

}

JobSystem::~JobSystem()
{

}

void JobSystem::ScheduleJob(Job* func, JobThreadType threadType)
{
    auto scheduleJobFunc = [&func](JobQueue& jobQueue)
    {
        std::lock_guard<std::mutex> lock(jobQueue.mutex_);
        jobQueue.jobs_.push_back(func);
        jobQueue.cv_.notify_one();
    };
	switch (threadType)
	{
    case JobThreadType::MAIN_THREAD:
    {
        func->Execute();
        break;
    }
    case JobThreadType::RENDER_THREAD:
	{
        scheduleJobFunc(renderJobs_);
        break;
    }
    case JobThreadType::RESOURCE_THREAD:
	{
        scheduleJobFunc(resourceJobs_);
    	break;
    }
    case JobThreadType::OTHER_THREAD:
	{
        scheduleJobFunc(jobs_);
        break;
    }
	default: ;
	}
}

void JobSystem::Work(JobQueue& jobQueue)
{
    {
        std::lock_guard<std::mutex> lock(statusMutex_);
        ++workersStarted_;

    }
    while (IsRunning())
    {
        Job* job = nullptr;
        {// CRITICAL
            std::unique_lock<std::mutex> lock(jobQueue.mutex_);

            if (!jobQueue.jobs_.empty())
            {
                job = jobQueue.jobs_.front();
                jobQueue.jobs_.erase(jobQueue.jobs_.cbegin());
                lock.unlock();
                if (!job->CheckDependenciesStarted())
                {
                    lock.lock();
                	if(jobQueue.jobs_.empty())
                	{
#ifdef EASY_PROFILE_USE
                        EASY_BLOCK("Wait for Dependencies");
#endif

                        jobQueue.cv_.wait_for(lock, std::chrono::microseconds(100));

                	}
                    jobQueue.jobs_.push_back(job);
                    continue;
                }
            }
            else
            {
                if (IsRunning()) // Atomic check.
                {
                    jobQueue.cv_.wait(lock); // !CRITICAL
                }
            	continue; //Refetch a new job

            }
        }// !CRITICAL
        job->Execute();
    }
}

void JobSystem::Init()
{
    status_ = RUNNING;
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    const auto minWorkerNumber = 3u;
    numberOfWorkers = config.workerNumber < minWorkerNumber ?
            std::max(minWorkerNumber, std::thread::hardware_concurrency() - 1):
            config.workerNumber;

    workers_.resize(numberOfWorkers);

    const size_t len = numberOfWorkers;
    for (size_t i = 0; i < len; ++i)
    {
        switch (i)
        {
            case static_cast<int>(JobThreadType::RENDER_THREAD):
            {
                workers_[i] = std::thread([this] { Work(renderJobs_); }); // Kick the thread => sys call
                break;
            }
            case static_cast<int>(JobThreadType::RESOURCE_THREAD):
            {
                workers_[i] = std::thread([this] { Work(resourceJobs_); }); // Kick the thread => sys call
                break;
            }
            default:
            {
                workers_[i] = std::thread([this] { Work(jobs_); }); // Kick the thread => sys call
                break;
            }
        }

    }
}

void JobSystem::Destroy()
{
    // Spin-lock waiting for all threads to become ready for shutdown.
    std::function<bool()> checkFunc = [this]()
    {
        std::lock_guard<std::mutex> lock(statusMutex_);
        return workersStarted_ != numberOfWorkers ||
                  !jobs_.jobs_.empty() ||
                  !renderJobs_.jobs_.empty() ||
                  !resourceJobs_.jobs_.empty();
    };
    while (checkFunc())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds (1));
    }
    status_ = NONE;
    renderJobs_.cv_.notify_all();
    resourceJobs_.cv_.notify_all();
	jobs_.cv_.notify_all(); // Wake all workers.
    const size_t len = numberOfWorkers;
    for (size_t i = 0; i < len; ++i)
    {
        workers_[i].join(); // Join all workers.
    }
}

bool JobSystem::IsRunning() const
{
    std::lock_guard<std::mutex> lock(statusMutex_);
    return status_ & Status::RUNNING;
}
std::uint8_t JobSystem::CountStartedWorkers() const
{
    std::lock_guard<std::mutex> lock(statusMutex_);
    return workersStarted_;
}

Job::Job(std::function<void()> task) :
        task_(std::move(task)),
        taskDoneFuture_(promise_.get_future()),
        status_(0)
{

}

Job::Job(Job&& job) noexcept
{
    dependencies_ = std::move(job.dependencies_);
    task_ = std::move(job.task_);
    status_ = job.status_;
    promise_ = std::move(job.promise_);
    taskDoneFuture_ = std::move(job.taskDoneFuture_);
}

Job& Job::operator=(Job&& job) noexcept
{
    dependencies_ = std::move(job.dependencies_);
    task_ = std::move(job.task_);
    status_ = job.status_;
    return *this;
}

void Job::Join() const
{
    if(!IsDone())
    {
        taskDoneFuture_.get();
    }
}


void Job::Execute()
{
    for (auto& dep : dependencies_)
    {
        if (!dep->IsDone())
        {
            dep->Join();
        }
    }

    {
        std::lock_guard<std::mutex> lock(statusLock_);
        status_ |= STARTED;
    }
    task_();
    {
        std::lock_guard<std::mutex> lock(statusLock_);
        status_ |= DONE;
    }
    promise_.set_value();
}

bool Job::CheckDependenciesStarted() const
{
    if (dependencies_.empty())
        return true;
	for(const auto& dep : dependencies_)
	{
		if(!dep->HasStarted())
            return false;
	}
    return true;
}

bool Job::HasStarted() const
{
    std::lock_guard<std::mutex> lock(statusLock_);
    return status_ & STARTED;
}

bool Job::IsDone() const
{
    std::lock_guard<std::mutex> lock(statusLock_);
    return status_ & DONE;
}

void Job::AddDependency(const Job* dependentJob)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Jobsystem Add Dependency");
#endif
    //Be sure to not create a cycle of dependencies which would deadlock the thread
	//Also check if the dependencies is not already in the dependency tree
    std::function<bool(const std::vector<const Job*>&, const Job*)> checkDependencies =
            [&checkDependencies, dependentJob](const std::vector<const Job*>& dependencies, const Job* job) {
                for(const auto& dep : dependencies)
                {
                    if(dep == job || dep == dependentJob)
                        return false;
                    const bool recursiveDep = checkDependencies(dep->dependencies_, job);
                    if(!recursiveDep)
                        return false;
                }
                return true;
            };
    if(checkDependencies(dependencies_, this))
    {
        dependencies_.push_back(dependentJob);
    }
}

void Job::Reset()
{
    promise_ = std::promise<void>();
    taskDoneFuture_ = promise_.get_future();
    status_ = NONE;
    dependencies_.clear();
}




}
