#include "engine/worker_system.h"

namespace neko
{

    Task::Task(std::function<void()> task) : task_(std::move(task)),
        status_(NONE),
        taskDoneFuture_(promise_.get_future())
    {

    }

    void Task::Join()
    {
        if (!IsDone())
        {
            taskDoneFuture_.get();
        }
    }

    void Task::Execute()
    {
        for (auto& depPtr : dependencies_)
        {
            std::shared_ptr<Task> dependency = depPtr.lock();
            if (dependency == nullptr)
                continue;

            if (!dependency->IsDone())
            {
                dependency->Join();
            }
        }
        status_ |= STARTED;
        task_();
        status_ |= DONE;
        promise_.set_value();
    }

    bool Task::CheckDependenciesStarted() const
    {
        if (dependencies_.empty())
            return true;
        for (const auto& depPtr : dependencies_)
        {
            std::shared_ptr<Task> dependency = depPtr.lock();
            if (dependency == nullptr)
                continue;
            if (!dependency->HasStarted())
                return false;
        }
        return true;
    }

    bool Task::IsDone() const
    {
        return status_ & DONE;
    }

    bool Task::HasStarted() const
    {
        return status_ & STARTED;
    }

    void Task::Reset()
    {
        promise_ = std::promise<void>();
        taskDoneFuture_ = promise_.get_future();
        status_ = NONE;
        dependencies_.clear();
    }

    void Task::AddDependency(const std::weak_ptr<Task>& newDependencyPtr)
    {
        auto dependency = newDependencyPtr.lock();
        //Be sure to not create a cycle of dependencies which would deadlock the thread
        //Also check if the dependencies is not already in the dependency tree
        std::function<bool(const std::vector<std::weak_ptr<Task>>&,
            const Task*)> checkDependencies =
            [&checkDependencies, dependency](
                const std::vector<std::weak_ptr<Task>>& dependencies,
                const Task* job) {
                    for (const auto& depPtr : dependencies)
                    {
                        auto dep = depPtr.lock();
                        if (dep.get() == job || dep == dependency)
                            return false;
                        const bool recursiveDep = checkDependencies(
                            dep->dependencies_, job);
                        if (!recursiveDep)
                            return false;
                    }
                    return true;
        };
        if (checkDependencies(dependencies_, this))
        {
            dependencies_.push_back(newDependencyPtr);
        }
    }


    WorkerThread::WorkerThread(WorkerQueue& queue) : taskQueue_(queue)
    {

    }

    void WorkerThread::Start()
    {
        thread_ = std::thread(&WorkerThread::Loop, this);
    }

    void WorkerThread::Loop()
    {
        while (taskQueue_.IsRunning())
        {
            if (taskQueue_.IsEmpty())
            {
                if (!taskQueue_.IsRunning())
                {
                    break;
                }
                taskQueue_.WaitForTask();
            }
            else
            {
                while (!taskQueue_.IsEmpty())
                {
                    auto newTask = taskQueue_.PopNextTask();
                    if(newTask == nullptr)
                        continue;
                    if (!newTask->CheckDependenciesStarted())
                    {
                        taskQueue_.AddTask(std::move(newTask));
                    }
                    else
                    {
                        newTask->Execute();
                    }
                }
            }
        }
    }
    
    
    

    void WorkerManager::Init(WorkerQueueDefinition mainQueue, std::initializer_list<WorkerQueueDefinition> otherQueues)
    {
        queues_.resize(otherQueues.size());
        int i = 0;
        for (auto& otherQueue : otherQueues)
        {
            queueMap_[otherQueue.name] = i;
            for(int j = 0; j < otherQueue.threadCount; j++)
            {
                threads_.emplace_back(queues_[i]);
            }
            i++;
        }
        queueMap_[mainQueue.name] = -1;
        std::for_each(threads_.begin(), threads_.end(), [](auto& thread) {thread.Start(); });
    }

    void WorkerManager::AddTask(const std::shared_ptr<Task>& task, std::string_view queueName)
    {
        const auto queueIndexIt = queueMap_.find(queueName.data());
        if (queueIndexIt == queueMap_.end())
        {
            return;
        }
        const auto queueIndex = queueIndexIt->second;
        if (queueIndex >= 0)
        {
            auto& queue = queues_[queueIndex];
            //Checking if already added
            if(queue.Contains(task))
            {
                return;
            }
            queues_[queueIndex].AddTask(task);

        }
        else
        {
            mainQueue_.AddTask(task);
        }
    }

    void WorkerManager::ExecuteMainThread()
    {
        while (!mainQueue_.IsEmpty())
        {
            auto newTask = mainQueue_.PopNextTask();
            if (!newTask->CheckDependenciesStarted())
            {
                mainQueue_.AddTask(std::move(newTask));
            }
            else
            {
                newTask->Execute();
            }
        }
    }

    void WorkerManager::Destroy()
    {
        std::for_each(queues_.begin(), queues_.end(), [](auto& queue) {queue.Destroy(); });
        std::for_each(threads_.begin(), threads_.end(), [](auto& thread) {thread.Destroy(); });
    }

    WorkerThread::~WorkerThread()
    {
        Destroy();
    }

    void WorkerThread::Destroy()
    {
        if (thread_.joinable())
        {
            thread_.join();
        }
    }

    WorkerQueue::WorkerQueue(WorkerQueue&& otherQueue) noexcept
    {
    }

    WorkerQueue& WorkerQueue::operator=(WorkerQueue&& otherQueue) noexcept
    {
        return *this;
    }

    bool WorkerQueue::IsEmpty() const
    {

        std::shared_lock<std::shared_mutex> lock(queueMutex_);
        return tasks_.empty();
    }

    bool WorkerQueue::IsRunning() const
    {
        std::shared_lock lock(queueMutex_);
        return isRunning_;
    }

    std::shared_ptr<Task> WorkerQueue::PopNextTask()
    {

        std::unique_lock<std::shared_mutex> lock(queueMutex_);
        if (tasks_.empty())
            return nullptr;
        auto task = tasks_.front();
        tasks_.erase(tasks_.cbegin());
        return task;
    }

    void WorkerQueue::AddTask(std::shared_ptr<Task> task)
    {
        std::unique_lock<std::shared_mutex> lock(queueMutex_);
        tasks_.push_back(std::move(task));
        conditionVariable_.notify_one();
    }

    void WorkerQueue::WaitForTask()
    {
        std::unique_lock<std::shared_mutex> lock(queueMutex_);
        conditionVariable_.wait(lock);
    }

    void WorkerQueue::Destroy()
    {
        {
            std::unique_lock lock(queueMutex_);
            isRunning_ = false;
        }
        conditionVariable_.notify_all();
    }

    WorkerQueue::~WorkerQueue()
    {
        Destroy();
    }

    bool WorkerQueue::Contains(const std::shared_ptr<Task>& task)
    {
        std::shared_lock<std::shared_mutex> lock(queueMutex_);
        return std::ranges::find(tasks_, task) != tasks_.end();
    }
}
