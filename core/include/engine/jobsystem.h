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
#include <array>
#include <queue>
#include <atomic>

namespace neko
{

class JobSystem
{
    enum Status : uint8_t
    { // Q: Why declaring this as an enum CLASS makes status_ & Status::RUNNING an invalid operation?
        RUNNING = 1u
    };

public:
    JobSystem()
    {
        for (size_t i = 0; i < DEFAULT_WORKER_SIZE; ++i)
        {
            tasks_.push([] {}); // Fill tasks queue with dummy functions to init workers.
        }
        for (size_t i = 0; i < DEFAULT_WORKER_SIZE; ++i)
        {
            workers_[i] = std::thread([this] { Work(); }); // Kick the thread => sys call
        }
    }

    ~JobSystem()
    {
        status_ = 0u; // Atomic assign.
        cv_.notify_all(); // Wake all workers.
        for (size_t i = 0; i < DEFAULT_WORKER_SIZE; ++i)
        {
            workers_[i].join();
        }
    }

    void KickJob(const std::function<void()>& func)
    {
        { // CRITICAL
            std::unique_lock<std::mutex> lock(mutex_);
            tasks_.push(func);
        } // !CRITICAL
        cv_.notify_one();
    }

    void Work()
    {
        NEXT_TASK:
        while (status_ & Status::RUNNING)
        {
            std::function<void()> task = [] {};
            { // CRITICAL
                std::unique_lock<std::mutex> lock(mutex_);
                if (!tasks_.empty())
                {
                    task = tasks_.front();
                    tasks_.pop();
                }
                else
                {
                    goto SLEEP;
                }
            } // !CRITICAL
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

private:
    const static size_t DEFAULT_WORKER_SIZE = 4;
    std::queue<std::function<void()>> tasks_; // Managed via mutex.
    std::array<std::thread, DEFAULT_WORKER_SIZE> workers_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<std::uint8_t> status_ = 1u;
};

}
