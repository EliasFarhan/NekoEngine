#include <engine/jobsystem.h>

#include <utility>

namespace neko
{

JobSystem::JobSystem()
{

}

JobSystem::~JobSystem()
{

}

void JobSystem::ScheduleJob(Job* func)
{
    {// CRITICAL
        std::unique_lock<std::mutex> lock(mutex_);
        jobs_.push(func);
    }// !CRITICAL
    cv_.notify_one();
}

void JobSystem::Work(std::queue<Job*>& jobs)
{
    initializedWorkers_++; // Atomic increment.

    while (status_ & Status::RUNNING) // Atomic check.
    {
        Job* job;
        {// CRITICAL
            std::unique_lock<std::mutex> lock(mutex_);
            if (!jobs.empty())
            {
                job = jobs.front();
                jobs.pop();
            }
            else
            {
                if (status_ & Status::RUNNING) // Atomic check.
                {
                    cv_.wait(lock); // !CRITICAL
                }
                continue;
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
    while (initializedWorkers_ != numberOfWorkers || !jobs_.empty())
    {} // WARNING: Not locking mutex for task_ access here!

    status_ = 0u; // Atomic assign.
    cv_.notify_all(); // Wake all workers.
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
}

bool Job::IsDone() const
{
    return status_ & DONE;
}

void Job::AddDependency(const Job* dep)
{
    //Be sure to not create a cycle of dependencies which would deadlock the thread
    std::function<bool(std::vector<const Job*>, const Job*)> checkDependencies =
            [&checkDependencies](std::vector<const Job*> dependencies, const Job* job) {
                for(auto& dep : dependencies)
                {
                    if(dep == job)
                        return false;
                    bool recursiveDep = checkDependencies(dep->dependencies_, job);
                    if(!recursiveDep)
                        return false;
                }
                return true;
            };
    if(checkDependencies(dependencies_, this))
    {
        dependencies_.push_back(dep);
    }
}

}
