#include <engine/jobsystem.h>

namespace neko
{

JobSystem::JobSystem()
{
    numberOfWorkers = std::thread::hardware_concurrency() - OCCUPIED_THREADS;
    //TODO@Oleg: Add neko assert to check number of worker threads is valid!
    workers_.reserve(numberOfWorkers);

    const size_t len = numberOfWorkers;
    for (size_t i = 0; i < len; ++i)
    {
        workers_[i] = std::thread([this] { Work(); }); // Kick the thread => sys call
    }
}

JobSystem::~JobSystem()
{
    // Spin-lock waiting for all threads to become ready for shutdown.
    while (initializedWorkers_ != numberOfWorkers || !tasks_.empty()){} // WARNING: Not locking mutex for task_ access here!

    status_ = 0u; // Atomic assign.
    cv_.notify_all(); // Wake all workers.
    const size_t len = numberOfWorkers;
    for (size_t i = 0; i < len; ++i)
    {
        workers_[i].join(); // Join all workers.
    }
}

void JobSystem::KickJob(const std::function<void()>& func)
{
    {// CRITICAL
        std::unique_lock<std::mutex> lock(mutex_);
        tasks_.push(func);
    }// !CRITICAL
    cv_.notify_one();
}

void JobSystem::Work()
{
    initializedWorkers_++; // Atomic increment.

    NEXT_TASK:
    while (status_ & Status::RUNNING) // Atomic check.
    {
        std::function<void()> task;
        {// CRITICAL
            std::unique_lock<std::mutex> lock(mutex_);
            if (!tasks_.empty())
            {
                task = tasks_.front();
                tasks_.pop();
            }
            else
            {
                goto SLEEP; // Exits CRITICAL.
            }
        }// !CRITICAL
        task();
        goto NEXT_TASK;

        SLEEP:
        if (status_ & Status::RUNNING) // Atomic check.
        {
            std::unique_lock<std::mutex> lock(mutex_); // CRITICAL
            cv_.wait(lock); // !CRITICAL
        }
    }
}

}
