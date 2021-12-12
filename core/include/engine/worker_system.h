#pragma once

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

#include <thread>
#include <condition_variable>
#include <future>
#include <shared_mutex>

#include "proto/worker.pb.h"

namespace neko
{

class Task
{
public:
    Task(std::function<void()> task);
    virtual ~Task() = default;
    Task(const Task&) = delete;
    Task& operator=(Task&) = delete;
    Task(Task&&) noexcept = delete;
    Task& operator=(Task&& job) noexcept = delete;
    void Join() const;
    void Execute();
    [[nodiscard]] bool CheckDependenciesStarted() const;
    [[nodiscard]] bool IsDone() const;
    [[nodiscard]] bool HasStarted() const;
    /**
     * \brief Add the given task as dependency and check not to create dependency cycle
     * \return True if it could add the dependency and false if could not
     */
    bool AddDependency(std::weak_ptr<Task> newDependencyPtr);
    /**
     * \brief This function clears the dependencies, the promise/future and status, but keeps the task function
     */
    virtual void Reset();
private:
    enum TaskStatus : std::uint8_t
    {
        NONE = 0u,
        STARTED = 1u << 0u,
        DONE = 1u << 1u,
    };
    std::function<void()> task_;
    std::atomic<std::uint8_t> status_;

    std::promise<void> promise_;
    std::shared_future<void> taskDoneFuture_;
    std::vector<std::weak_ptr<Task>> dependencies_;
};

class WorkerQueue
{
public:
    WorkerQueue() = default;
    ~WorkerQueue();
    bool Contains(const std::shared_ptr<Task>& task) const;
    WorkerQueue(const WorkerQueue&) = delete;
    WorkerQueue& operator= (const WorkerQueue&) = delete;
    WorkerQueue(WorkerQueue&&) noexcept;
    WorkerQueue& operator= (WorkerQueue&&) noexcept;

    [[nodiscard]] bool IsEmpty() const;
    [[nodiscard]] std::shared_ptr<Task> PopNextTask();
    bool AddTask(std::shared_ptr<Task> task);
    void WaitForTask();
    void Destroy();
    [[nodiscard]] bool IsRunning() const;
private:
    std::vector<std::shared_ptr<Task>> tasks_;
    mutable std::shared_mutex queueMutex_;
    std::condition_variable_any conditionVariable_;
    bool isRunning_ = true;
};


class WorkerThread
{
public:
    WorkerThread(WorkerQueue& queue);
    ~WorkerThread() = default;

    WorkerThread(const WorkerThread&) = delete;
    WorkerThread& operator= (const WorkerThread&) = delete;
    WorkerThread(WorkerThread&&) = default;
    WorkerThread& operator= (WorkerThread&&) = delete;

    void Start();
    /**
     * \brief Join with the owning thread. Should be called after WorkerQueue destruction
     */
    void Destroy();
private:
    void Loop() const;
    WorkerQueue& taskQueue_;
    std::thread thread_;
};

class WorkerManager
{
public:

    void Init(const core::pb::WorkerManager& workerManagerPb);
    bool AddTask(const std::shared_ptr<Task>& task, std::string_view queueName);
    void ExecuteMainThread();
    void Destroy();
    [[nodiscard]] std::uint8_t GetWorkersCount() const { return threads_.size(); }
private:
    WorkerQueue mainQueue_;
    std::vector<WorkerQueue> queues_;
    std::vector<WorkerThread> threads_;
    std::unordered_map<std::string, int> queueMap_;
};


}