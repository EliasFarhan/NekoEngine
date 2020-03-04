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

#include <atomic>
#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <mutex>
#include <thread>
#include <functional>
#include <utilities/service_locator.h>
#include <condition_variable>

namespace neko
{
//-----------------------------------------------------------------------------
// LogType
//-----------------------------------------------------------------------------
/// \brief To differentiate log messages
enum class LogType : std::uint8_t
{
	DEBUG = 1, //For regular debug messages
	WARNING, //For non-critical errors
	CRITICAL, //For critical errors
	LENGTH
};

//-----------------------------------------------------------------------------
// LogCategory
//-----------------------------------------------------------------------------
/// \brief To sort log messages into different categories
enum class LogCategory : std::uint8_t
{
	NONE = 1,
	ENGINE,
	MATH,
	GRAPHICS,
	IO,
	SOUND,
	LENGTH
};

//-----------------------------------------------------------------------------
// LogMessage
//-----------------------------------------------------------------------------
/// \brief Struct representing a log message with its type
struct LogMessage
{
	LogType type = LogType::DEBUG;
	LogCategory category = LogCategory::NONE;
	std::string log;

	explicit LogMessage(std::string log)
		: log(std::move(log))
	{
		Generate();
	}

	explicit LogMessage(const LogType& logType, std::string log)
		: type(logType), log(std::move(log))
	{
		Generate();
	}

	explicit LogMessage(const LogCategory& category, const LogType& logType,
	                    std::string log)
		: type(logType), category(category), log(std::move(log))
	{
		Generate();
	}

	void Generate();
	void Display() const
	{
		(type != LogType::CRITICAL ? std::cout : std::cerr) << log;
	}
};

//-----------------------------------------------------------------------------
// LogManagerInterface
//-----------------------------------------------------------------------------
/// \brief Used for the service locator
class LogManagerInterface
{
protected:
	~LogManagerInterface() = default;
public:
	/**
	 * \brief Generate a log message.
	 * @param logType the type of the log message
	 * @param log the log message
	 */
	virtual void Log(LogType logType, const std::string& log) = 0;

	/**
	 * \brief Generate a log message.
	 * @param logType the type of the log message
	 * @param category the category of the log message
	 * @param log the log message
	 */
	virtual void Log(LogCategory category, LogType logType,
	                 const std::string& log) = 0;

	/**
	 * \brief Retrieves the log history
	 */
	virtual const std::vector<LogMessage>& GetLogs() = 0;
};

//-----------------------------------------------------------------------------
// NullLogManager
//-----------------------------------------------------------------------------
/// \brief Used for the service locator
class NullLogManager final : public LogManagerInterface
{
	void Log([[maybe_unused]] LogType logType,
	         [[maybe_unused]] const std::string& log) override
	{}

	void Log([[maybe_unused]] LogCategory category,
	         [[maybe_unused]] LogType logType,
	         [[maybe_unused]] const std::string& log) override
	{}

	const std::vector<LogMessage>& GetLogs() override
	{
		std::cerr << "Impossible to get log history from a null LogManager\n";
		assert(false);
		return {};
	}
};

//-----------------------------------------------------------------------------
// LogManager
//-----------------------------------------------------------------------------
/// \brief Creates and stores log messages
class LogManager final : public LogManagerInterface
{
protected:
	//-----------------------------------------------------------------------------
	// LogManagerStatus
	//-----------------------------------------------------------------------------
	/// \brief To get the status of the engine
	enum LogManagerStatus : std::uint8_t
	{
		IS_RUNNING = 1u << 0u, //To check if the LogManager is running
		IS_EMPTY = 1u << 1u, //To check if the LogManager has tasks
		IS_LOG_WAITING = 1u << 2u, //To check if the LogManager is waiting for a task
		IS_WRITING = 1u << 3u //To check if the LogManager is writing its output to a file
	};
public:
	LogManager();
	~LogManager();

	void LogLoop();
	void Wait();
	void Destroy();

	void Log(LogType logType, const std::string& log) override;

	void Log(LogCategory category, LogType logType,
		const std::string& log) override;

	const std::vector<LogMessage>& GetLogs() override
	{
		return logHistory_;
	}

	void WriteToFile();
private:
	std::atomic<std::uint8_t> status_;

	std::vector<LogMessage> logHistory_;

	std::unique_ptr<std::thread> logThread_;
	std::mutex logMutex_;
	std::vector<std::function<void()>> tasks_;
	std::condition_variable conditionVariable_;
};

//-----------------------------------------------------------------------------
// Service Locator definition
//-----------------------------------------------------------------------------
using Log = Locator<LogManagerInterface, NullLogManager>;

//-----------------------------------------------------------------------------
// Shorthands
//-----------------------------------------------------------------------------
/**
 * \brief Generate a debug type log message
 */
void LogDebug(const std::string& msg);

/**
 * \brief Generate a debug type log message
 */
void LogDebug(LogCategory category, const std::string& msg);

/**
 * \brief Generate a warning type log message
 */
void LogWarning(const std::string& msg);

/**
 * \brief Generate a warning type log message
 */
void LogWarning(LogCategory category, const std::string& msg);

/**
 * \brief Generate an error type log message
 */
void LogError(const std::string& msg);

/**
 * \brief Generate an error type log message
 */
void LogError(LogCategory category, const std::string& msg);

/**
 * \brief Retrieves the log history
 */
const std::vector<LogMessage>& GetLogs();
}
