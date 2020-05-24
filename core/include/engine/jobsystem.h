#pragma once
/*
 MIT License
 Copyright (c) 2017 SAE Institute Switzerland AG
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

#include <functional>
#include <thread>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <future>
#include "engine/system.h"

namespace neko
{

enum class JobThreadType : int8_t
{
    MAIN_THREAD = -1,
    RENDER_THREAD = 0,
    RESOURCE_THREAD = 1,
    OTHER_THREAD = 2
};

class Job
{
public:
    enum class JobType
    {
        MANDATORY,
        DEPENDABLE,
        NICE_TO_HAVE
    };
    enum JobStatus : std::uint8_t
    {
        STARTED = 1u << 0u,
        DONE = 1u << 1u
    };
    explicit Job(std::function<void()> task);
    virtual ~Job() = default;
    Job(const Job&) = delete;
    Job& operator=(const Job&) = delete;
    Job(Job&& job) noexcept
    {
        promise_ = std::move(job.promise_);
        dependencies_ = std::move(job.dependencies_);
        task_ = std::move(job.task_);
        taskDoneFuture_ = std::move(job.taskDoneFuture_);
        status_ = job.status_.load();
    };
    Job& operator=(Job&& job) noexcept
    {
        promise_ = std::move(job.promise_);
        dependencies_ = std::move(job.dependencies_);
        task_ = std::move(job.task_);
        taskDoneFuture_ = std::move(job.taskDoneFuture_);
        status_ = job.status_.load();
        return *this;
    }
    /**
     * \brief Wait for the Job to be done,
     * used when dependencies are not done
     * useful when dependencies are on other threads
     */
    void Join() const;
    /**
     * \brief Execute is called by the JobSystem
     */
    void Execute();
	/**
	 *  \brief Check if all dependencies started
	 *  used when we want to start the job to know if we should join or wait for other dependencies
	 */
    bool CheckDependenciesStarted();
    bool IsDone() const;
    void AddDependency(const Job* dep);

    std::function<void()> GetTask() const { return task_; }
    void SetTask(std::function<void()> task) { task_ = task; }
    void Reset();

protected:
    std::vector<const Job*> dependencies_;
    std::function<void()> task_;
    std::promise<void> promise_;
    std::shared_future<void> taskDoneFuture_;
    std::atomic<std::uint8_t> status_;
};

struct JobQueue
{
    std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<Job*> jobs_;
};

class JobSystem : SystemInterface
{
    enum Status : uint8_t
    { // Q: Why declaring this as an enum CLASS makes status_ & Status::RUNNING an invalid operation?
        RUNNING = 1u
    };

public:
    JobSystem();
    ~JobSystem() override;
    void ScheduleJob(Job* func, JobThreadType threadType);
    void Init() override;

    void Update([[maybe_unused]]seconds dt) override{}

    void Destroy() override;

private:
	void Work(JobQueue& jobQueue);



    const static std::uint8_t OCCUPIED_THREADS = 3; // Define number of threads used by engine.
    std::uint8_t numberOfWorkers;
    JobQueue jobs_; // Managed via mutex. // TODO: replace with custom queue when those are implemented.
    JobQueue renderJobs_; // Managed via mutex. // TODO: replace with custom queue when those are implemented.
    JobQueue resourceJobs_; // Managed via mutex. // TODO: replace with custom queue when those are implemented.
    std::vector<std::thread> workers_; // TODO: replace with fixed vector when those are implemented.
    
    std::atomic<std::uint8_t> status_ = RUNNING;
    std::atomic<std::uint8_t> initializedWorkers_ = 0;
};

}
