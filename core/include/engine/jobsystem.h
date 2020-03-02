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

#include <utilities/service_locator.h>
#include <vector>
#include <functional>
#include <future>
#include <thread>
#include <queue>
#include <condition_variable>
#include <array>
#include <stack>

namespace neko
{

std::mutex mutex;
std::condition_variable cv;

/*template<typename ReturnType, typename... Args> class Job;
template<typename ReturnType, typename... Args> class Job<ReturnType(Args...)>
{
public:
    Job(std::function<ReturnType(Args...)> func, std::tuple<Args...> args): func_(func), args_(args) {};

    ReturnType Execute()
    {
        return std::apply(func_,args_);
    }
private:
    std::function<ReturnType(Args...)> func_;
    std::tuple<Args...> args_;
};

class JobSystem{
public:
    template<typename ReturnType, typename... Args>
    std::future<ReturnType> AddJob(std::function<ReturnType(Args...)> func, std::tuple<Args...> args){
        jobQueue_.emplace_back(Job<ReturnType(Args...)>(func, args));
        return std::promise<ReturnType>().get_future();
    }

    void RunJobs(){
        for (auto& job : jobQueue_){
            job.Execute();
        }
    }
private:
    std::vector<Job> jobQueue_;
};*/

void Work(std::function<void(void)> (* GrabJob)())
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock);

        GrabJob()(); // Grab job and execute
    }
}

class JobSystem
{
public:
    JobSystem()
    {
        for (int i = 0; i < DEFAULT_WORKER_SIZE; ++i)
        {
            workers_[i] = std::thread(Work, [this] { GrabJob(); });
        }
    }

    void AddJob(std::function<void(void)> job)
    {
        jobQueue_.push(job);
    }

    void ExecuteJobs()
    {
        const size_t queueSize = jobQueue_.size();
        for (int i = 0; i < queueSize; ++i)
        {
            cv.notify_one();
        }
    }

    std::function<void(void)> GrabJob()
    {
        auto job = jobQueue_.top();
        jobQueue_.pop();
        return job;
    }

private:
    const static size_t DEFAULT_WORKER_SIZE = 2;
    std::stack<std::function<void(void)>> jobQueue_;
    std::array<std::thread, DEFAULT_WORKER_SIZE> workers_;
};

}
