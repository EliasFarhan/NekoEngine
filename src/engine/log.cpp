#include <engine/log.h>
#include <atomic>
#include <iostream>
#include <chrono>
#include <ctime>

class DebugLogger
{
public:
	explicit DebugLogger();
	void write(std::string msg);
	void logLoop();
	std::atomic<bool> isRunning = true;
	std::condition_variable newMsgSync;
private:
	std::thread logThread;
	std::queue<std::string> msgQueue;
	std::mutex msgMutex;

};


static DebugLogger* debugLogger = nullptr;

DebugLogger::DebugLogger()
{
	{
		std::chrono::time_point<std::chrono::system_clock> end;
		end = std::chrono::system_clock::now();
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		std::ofstream logFile("neko_log.txt", std::ios::out | std::ios::app);
		logFile << "\nNeko Engine : " << std::ctime(&end_time)<<"\n";
	}
	logThread = std::thread(&DebugLogger::logLoop, this);
	logThread.detach();
}

void DebugLogger::write(std::string msg)
{
	std::unique_lock<std::mutex> lock(msgMutex);
	msgQueue.push(msg);
	newMsgSync.notify_one();
}

void DebugLogger::logLoop()
{
	while(isRunning)
	{
		
		{
			std::unique_lock<std::mutex> lock(msgMutex);
			newMsgSync.wait(lock);
		}
		{
			std::ofstream logFile( "neko_log.txt",std::ios::out | std::ios::app);
			while (!msgQueue.empty())
			{
				std::string msg;
				{
					std::unique_lock<std::mutex> lock(msgMutex);
					msg = msgQueue.front();
					msgQueue.pop();
				}
				logFile << msg << "\n";
				std::cout << msg << "\n";
			}
		}
	}
	delete this;
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

	debugLogger->isRunning = false;
	debugLogger->newMsgSync.notify_one();
	debugLogger = nullptr;
}
