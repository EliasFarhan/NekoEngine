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
	struct tm localTime {};
	time_t curTime = time(nullptr);
	localtime_s(&localTime, &curTime);
	std::ostringstream message;
	message << '[' << std::setw(2) << std::setfill('0') << localTime.tm_hour << ":"
		<< std::setw(2) << std::setfill('0') << localTime.tm_min << ":"
		<< std::setw(2) << std::setfill('0') << localTime.tm_sec << "] ";

	switch (category)
	{
	case LogCategories::NONE:
		break;
	case LogCategories::ENGINE:
		message << "[ENGINE] ";
		break;
	case LogCategories::MATH:
		message << "[MATH] ";
		break;
	case LogCategories::GRAPHICS:
		message << "[GRAPHICS] ";
		break;
	case LogCategories::IO:
		message << "[IO] ";
		break;
	case LogCategories::SOUND:
		message << "[SOUND] ";
		break;
	}

	switch (type)
	{
	case LogTypes::DEBUG:
		message << "[DEBUG] ";
		break;
	case LogTypes::WARNING:
		message << "[WARNING] ";
		break;
	case LogTypes::CRITICAL:
		message << "[ERROR] ";
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
	logThread_ = std::make_unique<std::thread>([this]
	{
		status_ |= IS_RUNNING;
		this->Start();
	});
}

LogManager::~LogManager()
{
	WriteToFile();
	Destroy();
}

void LogManager::Start()
{
	do {
		if (tasks_.empty()) status_ |= IS_EMPTY;
		if (status_ & IS_EMPTY)
		{
			std::unique_lock<std::mutex> lock(logMutex_);
			
			status_ |= IS_APP_WAITING;
			itemInQueue_.wait(lock);
			status_ &= ~IS_APP_WAITING;
		}
		else
		{
			std::lock_guard<std::mutex> lock(logMutex_);
			const auto task = tasks_.front();
			tasks_.erase(tasks_.begin());
			task();
			itemInQueue_.notify_one();
		}
	} while (status_ & IS_RUNNING);
}

void LogManager::Wait()
{
	std::unique_lock<std::mutex> lock(logMutex_);
	while (!(status_ & IS_EMPTY))
	{
		if (tasks_.empty()) status_ |= IS_EMPTY;
		else
		{
			status_ |= IS_APP_WAITING;
			itemInQueue_.wait(lock);
			status_ &= ~IS_APP_WAITING;
		}
	}
	lock.unlock();
}

void LogManager::Destroy()
{
	std::unique_lock<std::mutex> lock(logMutex_);
	if (status_ & IS_WRITING)
	{
		status_ |= IS_APP_WAITING;
		itemInQueue_.wait(lock);
		status_ &= ~IS_APP_WAITING;
	}
	
	{
		itemInQueue_.notify_one();
		
		status_ &= ~IS_RUNNING;
	}
	lock.unlock();

	logThread_->join();
}

void LogManager::Log(LogTypes logType, const std::string& log)
{
	Log(LogCategories::NONE, logType, log);
}


void LogManager::Log(LogCategories category, LogTypes logType,
	const std::string& log)
{
	std::unique_lock<std::mutex> lock(logMutex_);
	status_ &= ~IS_EMPTY;

	auto task([this, logType, category, log]
	{
		LogMessage message(category, logType, log);
		message.Display();
		
		logHistory_.emplace_back(message);
	});

	tasks_.emplace_back(task);
	itemInQueue_.notify_one();
	lock.unlock();
}

void LogManager::WriteToFile()
{
	std::lock_guard<std::mutex> lock(logMutex_);
	
	status_ |= IS_WRITING;
	status_ &= ~IS_EMPTY;

	auto task([this]
	{
		struct tm localTime {};
		time_t curTime = time(nullptr);
		localtime_s(&localTime, &curTime);

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

		LogMessage message(LogCategories::IO, LogTypes::DEBUG, "Successfully saved log output");
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
	itemInQueue_.notify_one();
}

//-----------------------------------------------------------------------------
// Shorthands definitions
//-----------------------------------------------------------------------------
void LogDebug(const std::string& msg)
{
	Log::get().Log(LogTypes::DEBUG, msg);
}

void LogDebug(const LogCategories category, const std::string& msg)
{
	Log::get().Log(category, LogTypes::DEBUG, msg);
}

void LogWarning(const std::string& msg)
{
	Log::get().Log(LogTypes::WARNING, msg);
}

void LogWarning(const LogCategories category, const std::string& msg)
{
	Log::get().Log(category, LogTypes::WARNING, msg);
}

void LogError(const std::string& msg)
{
	Log::get().Log(LogTypes::CRITICAL, msg);
}

void LogError(const LogCategories category, const std::string& msg)
{
	Log::get().Log(category, LogTypes::CRITICAL, msg);
}

const std::vector<LogMessage>& GetLogs()
{
	return Log::get().GetLogs();
}
}
