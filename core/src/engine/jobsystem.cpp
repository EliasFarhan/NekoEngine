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
        std::unique_lock<std::mutex> lock(jobQueue.mutex_);
        jobQueue.jobs_.push(func);
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
    ++initializedWorkers_; // Atomic increment.

    while (status_ & Status::RUNNING) // Atomic check.
    {
        Job* job = nullptr;
        {// CRITICAL
            std::unique_lock<std::mutex> lock(jobQueue.mutex_);
            if (!jobQueue.jobs_.empty())
            {
                job = jobQueue.jobs_.front();
                jobQueue.jobs_.pop();
                if (!job->CheckDependenciesStarted())
                {
                	if(jobQueue.jobs_.empty())
                	{
#ifdef EASY_PROFILE_USE
                        EASY_BLOCK("Wait for Dependencies");
#endif
                        jobQueue.cv_.wait_for(lock, std::chrono::microseconds(100));
                	}
                    jobQueue.jobs_.push(job);
                    continue;
                }
            }
            else
            {
                if (status_ & Status::RUNNING) // Atomic check.
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
    numberOfWorkers = std::thread::hardware_concurrency() - 1;
    //TODO@Oleg: Add neko assert to check number of worker threads is valid!
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
    while (initializedWorkers_ != numberOfWorkers ||
        !jobs_.jobs_.empty() ||
        !renderJobs_.jobs_.empty() ||
        !resourceJobs_.jobs_.empty())
    {} // WARNING: Not locking mutex for task_ access here!

    status_ = 0u; // Atomic assign.
    renderJobs_.cv_.notify_all();
    resourceJobs_.cv_.notify_all();
	jobs_.cv_.notify_all(); // Wake all workers.
    const size_t len = numberOfWorkers;
    for (size_t i = 0; i < len; ++i)
    {
        workers_[i].join(); // Join all workers.
    }
}


Job::Job(std::function<void()> task) :
        task_(std::move(task)),
        taskDoneFuture_(promise_.get_future()),
        status_(0)
{

}

void Job::Join() const
{
    if(!(status_ & DONE))
        taskDoneFuture_.get();
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

    status_ |= STARTED;
    task_();
    status_ |= DONE;
    promise_.set_value();
}

bool Job::CheckDependenciesStarted()
{
	for(auto& dep : dependencies_)
	{
		if(!(dep->status_ & STARTED))
            return false;
	}
    return true;
}

bool Job::IsDone() const
{
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
                for(auto& dep : dependencies)
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
    status_ = 0;
    dependencies_.clear();
}
}
