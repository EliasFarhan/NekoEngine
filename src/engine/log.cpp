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
private:
    std::thread logThread;
    std::vector<std::string> msgQueue;

};


static DebugLogger* debugLogger = nullptr;

DebugLogger::DebugLogger()
{
    {
        std::chrono::time_point<std::chrono::system_clock> end;
        end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::ofstream logFile("neko_log.txt", std::ios::out | std::ios::app);
        logFile << "\nNeko Engine : " << std::ctime(&end_time) << "\n";
    }
    logThread = std::thread(&DebugLogger::logLoop, this);
    logThread.detach();
}

void DebugLogger::write(std::string msg)
{
    {
        std::unique_lock<std::mutex> lock(msgMutex);
        msgQueue.push_back(msg);
    }
    newMsgSync.notify_one();
}

void DebugLogger::logLoop()
{
    while (isRunning)
    {

        {
            std::unique_lock<std::mutex> lock(msgMutex);
            newMsgSync.wait(lock);
        }
        {
            std::ofstream logFile("neko_log.txt", std::ios::out | std::ios::app);
            while (!msgQueue.empty())
            {
                std::string msg;
                {
                    std::unique_lock<std::mutex> lock(msgMutex);
                    msg = msgQueue.front();
					msgQueue.erase(msgQueue.begin());
                }
                logFile << msg << "\n";
                std::cout << msg << "\n";
            }
        }
    }
	newMsgSync.notify_all();
}

void initLog()
{
    debugLogger = new DebugLogger();
}

void logDebug(std::string msg)
{
    debugLogger->write(msg);

}

void destroyLog()
{
    debugLogger->newMsgSync.notify_all();
	debugLogger->isRunning = false;
    {
		std::unique_lock<std::mutex> lock(debugLogger->msgMutex);
		debugLogger->newMsgSync.wait(lock);
    }
	delete(debugLogger);
	debugLogger = nullptr;
}
