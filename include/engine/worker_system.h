#pragma once

#include <thread>
#include <condition_variable>
#include <future>
#include <shared_mutex>

#ifdef TRACY_ENABLE
#include "Tracy.hpp"
#endif
namespace neko
{

    class Task
    {
    public:
        Task(std::function<void()> task);
        Task(const Task&) = delete;
        Task& operator=(Task&) = delete;
        Task(Task&&) noexcept = delete;
        Task& operator=(Task&& job) noexcept = delete;
        void Join();
        void Execute();
        [[nodiscard]] bool CheckDependenciesStarted() const;
        [[nodiscard]] bool IsDone() const;
        [[nodiscard]] bool HasStarted() const;
        void AddDependency(const std::weak_ptr<Task>& newDependencyPtr);
        /**
         * \brief This function clears the dependencies, the promise/future and status, but keeps the task function
         */
        void Reset();
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
        bool Contains(const std::shared_ptr<Task>& task);
        WorkerQueue(const WorkerQueue&) = delete;
        WorkerQueue& operator= (const WorkerQueue&) = delete;
        WorkerQueue(WorkerQueue&&) noexcept;
        WorkerQueue& operator= (WorkerQueue&&) noexcept;

        [[nodiscard]] bool IsEmpty() const;
        [[nodiscard]] std::shared_ptr<Task> PopNextTask();
        [[nodiscard]] bool IsRunning() const;
        void AddTask(std::shared_ptr<Task> task);
        void WaitForTask();
        void Destroy();
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
        ~WorkerThread();

        WorkerThread(const WorkerThread&) = delete;
        WorkerThread& operator= (const WorkerThread&) = delete;
        WorkerThread(WorkerThread&&) = default;
        WorkerThread& operator= (WorkerThread&&) = delete;

        void Start();
        void Destroy();
    private:
        void Loop();
        WorkerQueue& taskQueue_;
        std::thread thread_;
    };

    struct WorkerQueueDefinition
    {
        std::string name;
        std::size_t threadCount;
    };

    class WorkerManager
    {
    public:

        void Init(WorkerQueueDefinition mainQueue, std::initializer_list<WorkerQueueDefinition> otherQueues);
        void AddTask(const std::shared_ptr<Task>& task, std::string_view queueName);
        void ExecuteMainThread();
        void Destroy();
    private:
        WorkerQueue mainQueue_;
        std::vector<WorkerQueue> queues_;
        std::vector<WorkerThread> threads_;
        std::unordered_map<std::string, int> queueMap_;
    };
} // namespace neko
