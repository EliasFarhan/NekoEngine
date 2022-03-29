/*
 MIT License

 Copyright (c) 2021 SAE Institute Switzerland AG

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

#include "engine/worker_system.h"

#include "engine/log.h"
#include <fmt/core.h>
#include "engine/assert.h"


namespace neko
{

Task::Task(std::function<void()> task) : task_(std::move(task)),
                                         status_(NONE),
                                         taskDoneFuture_(promise_.get_future())
{

}

void Task::Join() const
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
    return std::all_of(dependencies_.begin(), dependencies_.end(), [](const auto& depPtr)
        {
            auto dependency = depPtr.lock();
            return dependency == nullptr || dependency->HasStarted();
        });
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

bool Task::AddDependency(std::weak_ptr<Task> newDependencyPtr)
{
    auto dependency = newDependencyPtr.lock();
    if (dependency == nullptr)
        return false;
    if (dependency.get() == this)
        return false;
    //Be sure to not create a cycle of dependencies which would deadlock the thread
    //Also check if the dependencies is not already in the dependency tree
    std::function<bool(const std::vector<std::weak_ptr<Task>>&,
                       const Task*)> checkDependencies =
            [&checkDependencies, dependency](
                    const std::vector<std::weak_ptr<Task>>& dependencies,
                    const Task* job) {
                const bool badDependency = std::any_of(dependencies.begin(), dependencies.end(), 
                    [job, dependency, &checkDependencies](const auto& depPtr)
                {
                        auto dep = depPtr.lock();
                        if (dep == nullptr)
                            return false;
                        if (dep.get() == job || dep == dependency)
                            return true;
                        const bool recursiveDep = checkDependencies(
                            dep->dependencies_, job);
                        if (!recursiveDep)
                            return true;
                        return false;
                });
                return !badDependency;
            };
    if (checkDependencies(dependencies_, this) && checkDependencies(dependency->dependencies_, this))
    {
        dependencies_.push_back(newDependencyPtr);
        return true;
    }
    return false;
}


WorkerThread::WorkerThread(WorkerQueue& queue) : taskQueue_(queue)
{

}

void WorkerThread::Start()
{
    thread_ = std::thread(&WorkerThread::Loop, this);
}

void WorkerThread::Loop() const
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

void WorkerManager::Init(const core::pb::WorkerManager& workerManagerPb)
{
    queues_.resize(workerManagerPb.other_queues_size());
    for(int i = 0; i < workerManagerPb.other_queues_size(); i++)
    {
        const auto& queue = workerManagerPb.other_queues(i);
        queueMap_[queue.name()] = i;
    }
    queueMap_[workerManagerPb.main_queue().name()] = -1;
    for (int i = 0; i < workerManagerPb.other_threads_size(); i++)
    {
        auto& thread = workerManagerPb.other_threads(i);
        auto& queue = thread.queue();
        const auto queueIndex = queueMap_[queue.name()];
        for (int i = 0; i < thread.thread_count(); i++)
        {
            threads_.emplace_back(queueIndex >= 0 ? queues_[queueIndex] : mainQueue_);
        }
    }
    std::for_each(threads_.begin(), threads_.end(), [](auto& thread) {thread.Start(); });

}

bool WorkerManager::AddTask(const std::shared_ptr<Task>& task, std::string_view queueName)
{
    const auto queueIndexIt = queueMap_.find(queueName.data());
    if(queueIndexIt == queueMap_.end())
    {
        logWarning(fmt::format("Could not add task to queue: {}, it does not exist", queueName));
        return false;
    }
    const auto queueIndex = queueIndexIt->second;
    if(queueIndex >= 0)
    {
        queues_[queueIndex].AddTask(task);
    }
    else
    {
        mainQueue_.AddTask(task);
    }
    return true;
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

core::pb::WorkerManager WorkerManager::CreateWorkerManagerDefinition()
{
    core::pb::WorkerManager workerManager;

    auto* mainQueue = workerManager.mutable_main_queue();
    *mainQueue->mutable_name() = WorkerQueue::MAIN_QUEUE_NAME;
    auto* renderQueue = workerManager.add_other_queues();
    *renderQueue->mutable_name() = WorkerQueue::RENDER_QUEUE_NAME;
    
    auto* resourceQueue = workerManager.add_other_queues();
    *resourceQueue->mutable_name() = WorkerQueue::RESOURCE_QUEUE_NAME;
    auto* otherQueue = workerManager.add_other_queues();
    *otherQueue->mutable_name() = WorkerQueue::OTHER_QUEUE_NAME;

    auto* renderThread = workerManager.add_other_threads();
    renderThread->set_thread_count(1);
    *renderThread->mutable_queue()->mutable_name() = WorkerQueue::RENDER_QUEUE_NAME;
    auto* resourceThread = workerManager.add_other_threads();
    resourceThread->set_thread_count(1);
    *resourceThread->mutable_queue()->mutable_name() = WorkerQueue::RESOURCE_QUEUE_NAME;
    auto* otherThread = workerManager.add_other_threads();
    otherThread->set_thread_count(std::thread::hardware_concurrency() - 2);
    *otherThread->mutable_queue()->mutable_name() = WorkerQueue::OTHER_QUEUE_NAME;

    return workerManager;
}

void WorkerThread::Destroy()
{
    neko_assert(!taskQueue_.IsRunning(), "Task Queue is not supposed to be running anymore");
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
    std::shared_lock lock(queueMutex_);
    return tasks_.empty();
}

std::shared_ptr<Task> WorkerQueue::PopNextTask()
{
    if (IsEmpty())
        return nullptr;
    std::unique_lock lock(queueMutex_);
    auto task = tasks_.front();
    tasks_.erase(tasks_.cbegin());
    return task;
}

bool WorkerQueue::AddTask(std::shared_ptr<Task> task)
{
    if (Contains(task))
    {
        logWarning("Worker Queue already contains added task");
        return false;
    }
    std::unique_lock lock(queueMutex_);
    tasks_.push_back(std::move(task));
    conditionVariable_.notify_one();
    return true;
}

void WorkerQueue::WaitForTask()
{
    std::unique_lock lock(queueMutex_);
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

bool WorkerQueue::IsRunning() const
{
    std::shared_lock lock(queueMutex_);
    return isRunning_;
}

WorkerQueue::~WorkerQueue()
{
    Destroy();
}

bool WorkerQueue::Contains(const std::shared_ptr<Task>& task) const
{
    std::shared_lock lock(queueMutex_);
    return std::any_of(tasks_.cbegin(), tasks_.cend(), [task](const auto& intern_task)
    {
        return intern_task == task;
    });
}
}
