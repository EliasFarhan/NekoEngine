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

namespace neko
{

class JobSystem
{
    enum Status : uint8_t
    { // Q: Why declaring this as an enum CLASS makes status_ & Status::RUNNING an invalid operation?
        RUNNING = 1u
    };

public:
    JobSystem();
    ~JobSystem();
    void KickJob(const std::function<void()>& func);
    void Work();

private:
    const static std::uint8_t OCCUPIED_THREADS = 3; // Define number of threads used by engine.
    std::uint8_t numberOfWorkers;
    std::queue<std::function<void()>> tasks_; // Managed via mutex. // TODO: replace with custom queue when those are implemented.
    std::vector<std::thread> workers_; // TODO: replace with fixed vector when those are implemented.
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<std::uint8_t> status_ = 1u;
    std::atomic<std::uint8_t> initializedWorkers_ = 0;
};

}
