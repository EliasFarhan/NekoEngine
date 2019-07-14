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
#include <engine/log.h>
#include <memory>
#include <atomic>
#include <iostream>
#include <chrono>
#include <ctime>
#include <vector>
#include <Remotery.h>

class DebugLogger
{
public:
    explicit DebugLogger();

    void write(std::string msg);

    void logLoop();

    std::atomic<bool> isRunning = true;
    std::condition_variable newMsgSync;

	std::mutex msgMutex;
	std::mutex loggingMutex;
private:
    std::thread logThread_;
    std::vector<std::string> msgQueue_;

};


static std::unique_ptr<DebugLogger> debugLogger = nullptr;

DebugLogger::DebugLogger()
{
    {
	    const auto end = std::chrono::system_clock::now();
        const auto end_time = std::chrono::system_clock::to_time_t(end);
        std::ofstream logFile("neko_log.txt", std::ios::out | std::ios::app);
        logFile << "\nNeko Engine : " << std::ctime(&end_time) << "\n";
    }
    logThread_ = std::thread(&DebugLogger::logLoop, this);
    logThread_.detach();
}

void DebugLogger::write(std::string msg)
{
    {
        std::unique_lock<std::mutex> lock(msgMutex);
        msgQueue_.push_back(msg);
    }
    newMsgSync.notify_one();
}

void DebugLogger::logLoop()
{
    while (isRunning)
    {
        {
            std::unique_lock<std::mutex> lock(msgMutex);
            using namespace std::chrono_literals;
            newMsgSync.wait_for(lock, 1ms);
        }
        {
            std::unique_lock<std::mutex> lock(loggingMutex);
            bool msgQueueEmpty = true;
            {
                std::unique_lock<std::mutex> checkingQueueLock(msgMutex);
                msgQueueEmpty = msgQueue_.empty();
            }
            if(!msgQueueEmpty)
            {
                std::ofstream logFile("neko_log.txt", std::ios::out | std::ios::app);
                while (!msgQueueEmpty)
                {
                    std::string msg;
                    {
                        std::unique_lock<std::mutex> masgLock(msgMutex);
                        msg = msgQueue_.front();
                        msgQueue_.erase(msgQueue_.begin());
                    }
                    logFile << msg << "\n";
                    std::cout << msg << std::endl;
                    {
                        std::unique_lock<std::mutex> msgLock(msgMutex);
                        msgQueueEmpty = msgQueue_.empty();
                    }

                }
            }
        }
    }
	newMsgSync.notify_all();
}

void initLog()
{
    debugLogger = std::make_unique<DebugLogger>();
}

void logDebug(std::string msg)
{
    debugLogger->write(msg);

}

void destroyLog()
{

    debugLogger->isRunning = false;
    debugLogger->newMsgSync.notify_all();
    {
		std::unique_lock<std::mutex> lock(debugLogger->loggingMutex);
        using namespace std::chrono_literals;
		debugLogger->newMsgSync.wait_for(lock, 5ms);
    }
	debugLogger = nullptr;
}
