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
#include <utilities/file_utility.h>

#include <memory>
#include <iomanip>
#include <sstream>

namespace neko
{
//-----------------------------------------------------------------------------
// LogMessage definitions
//-----------------------------------------------------------------------------
void LogMessage::Generate()
{
    time_t curTime = time(nullptr);
#ifdef _MSC_VER
	struct tm localTime {};
	localtime_s(&localTime, &curTime);
#else
    tm localTime = *localtime(&curTime);
#endif
	std::ostringstream message;
	message << '[' << std::setw(2) << std::setfill('0') << localTime.tm_hour << ":"
		<< std::setw(2) << std::setfill('0') << localTime.tm_min << ":"
		<< std::setw(2) << std::setfill('0') << localTime.tm_sec << "] ";

	switch (type)
	{
	case LogType::DEBUG:
		message << "[DEBUG] ";
		break;
	case LogType::WARNING:
		message << "[WARNING] ";
		break;
	case LogType::CRITICAL:
		message << "[ERROR] ";
		break;
	default:
		break;
	}
	
	switch (category)
	{
	case LogCategory::ENGINE:
		message << "[ENGINE] ";
		break;
	case LogCategory::MATH:
		message << "[MATH] ";
		break;
	case LogCategory::GRAPHICS:
		message << "[GRAPHICS] ";
		break;
	case LogCategory::IO:
		message << "[IO] ";
		break;
	case LogCategory::SOUND:
		message << "[SOUND] ";
		break;
	default:
		break;
	}

	message << log << '\n';
	log = message.str();
}

//-----------------------------------------------------------------------------
// LogManager definitions
//-----------------------------------------------------------------------------
LogManager::LogManager()
	: status_(0)
{
	Log::provide(this);
	status_ |= IS_RUNNING | IS_EMPTY;
	logThread_ = std::make_unique<std::thread>([this]
	{
		LogLoop();
	});
}

LogManager::~LogManager()
{
	WriteToFile();
	Destroy();
}

void LogManager::LogLoop()
{
	do {
		if (status_ & IS_EMPTY)
		{
			std::unique_lock<std::mutex> lock(logMutex_);
			
			status_ |= IS_LOG_WAITING;
			conditionVariable_.wait(lock);
			status_ &= ~IS_LOG_WAITING;
		}
		else
		{
			std::function<void()> task;
			{
				std::lock_guard<std::mutex> lock(logMutex_);
				task = tasks_.front();
				tasks_.erase(tasks_.begin());
				if (tasks_.empty()) status_ |= IS_EMPTY;
			}
			task();
			conditionVariable_.notify_one();
		}
	} while (status_ & IS_RUNNING);
}

void LogManager::Wait()
{
	while (!(status_ & IS_EMPTY))
	{
		std::unique_lock<std::mutex> lock(logMutex_);
		if (tasks_.empty()) status_ |= IS_EMPTY;
		else
		{
			status_ |= IS_LOG_WAITING;
			conditionVariable_.wait(lock);
			status_ &= ~IS_LOG_WAITING;
		}
	}
}

void LogManager::Destroy()
{
	std::unique_lock<std::mutex> lock(logMutex_);
	if (status_ & IS_WRITING)
	{
		status_ |= IS_LOG_WAITING;
		conditionVariable_.wait(lock);
		status_ &= ~IS_LOG_WAITING;
	}
	
	{
		conditionVariable_.notify_one();
		
		status_ &= ~IS_RUNNING;
	}
	lock.unlock();

	logThread_->join();
}

void LogManager::Log(LogType logType, const std::string& log)
{
	Log(LogCategory::NONE, logType, log);
}


void LogManager::Log(LogCategory category, LogType logType,
	const std::string& log)
{
	std::lock_guard<std::mutex> lock(logMutex_);
	status_ &= ~IS_EMPTY;

	auto task([this, logType, category, log]
	{
		LogMessage message(category, logType, log);
		message.Display();
		
		logHistory_.emplace_back(message);
	});

	tasks_.emplace_back(task);
	conditionVariable_.notify_one();
}

void LogManager::WriteToFile()
{
	std::lock_guard<std::mutex> lock(logMutex_);
	
	status_ |= IS_WRITING;
	status_ &= ~IS_EMPTY;

	auto task([this]
	{
        time_t curTime = time(nullptr);
#ifdef _MSC_VER
        struct tm localTime {};
	    localtime_s(&localTime, &curTime);
#else
	    tm localTime = *localtime(&curTime);
#endif

		const std::string filePath = "../data/logs/";
		std::ostringstream dateTime;
		dateTime << std::setw(2) << std::setfill('0') << localTime.tm_mday << "-"
			<< std::setw(2) << std::setfill('0') << localTime.tm_mon + 1 << "-"
			<< std::to_string(localTime.tm_year + 1900) << "_"
			<< std::setw(2) << std::setfill('0') << localTime.tm_hour << "-"
			<< std::setw(2) << std::setfill('0') << localTime.tm_min << "-"
			<< std::setw(2) << std::setfill('0') << localTime.tm_sec;

		std::string fileContent =
			"/--------------------------------------------------------------------------------\\\n";
		fileContent +=
			"|                                NekoEngine logs                                 |\n";
		fileContent += "|                              " + dateTime.str() + "                               |\n";
		fileContent +=
			"|              Copyright (c) 2017-2020 SAE Institute Switzerland AG              |\n";
		fileContent +=
			"\\--------------------------------------------------------------------------------/\n\n";

		fileContent += "Program start (=^ ◡ ^=)\n";
		fileContent +=
			"--------------------------------------------------------------------------------\n";

		LogMessage message(LogCategory::IO, LogType::DEBUG, "Successfully saved log output");
		logHistory_.emplace_back(message);
		message.Display();
		
		for (auto& line : logHistory_)
		{
			fileContent += line.log;
		}

		CreateDirectory(filePath);
		WriteStringToFile(filePath + dateTime.str() + ".log", fileContent);

		status_ &= ~IS_WRITING;
	});

	tasks_.emplace_back(task);
	conditionVariable_.notify_one();
}

//-----------------------------------------------------------------------------
// Shorthands definitions
//-----------------------------------------------------------------------------
void LogDebug(const std::string& msg)
{
	Log::get().Log(LogType::DEBUG, msg);
}

void LogDebug(const LogCategory category, const std::string& msg)
{
	Log::get().Log(category, LogType::DEBUG, msg);
}

void LogWarning(const std::string& msg)
{
	Log::get().Log(LogType::WARNING, msg);
}

void LogWarning(const LogCategory category, const std::string& msg)
{
	Log::get().Log(category, LogType::WARNING, msg);
}

void LogError(const std::string& msg)
{
	Log::get().Log(LogType::CRITICAL, msg);
}

void LogError(const LogCategory category, const std::string& msg)
{
	Log::get().Log(category, LogType::CRITICAL, msg);
}

const std::vector<LogMessage>& GetLogs()
{
	return Log::get().GetLogs();
}
}
