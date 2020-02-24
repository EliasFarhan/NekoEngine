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

#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <mutex>
#include <thread>
#include <queue>
#include <functional>
#include <utilities/service_locator.h>
#include <condition_variable>

namespace neko
{
//-----------------------------------------------------------------------------
// LogTypes
//-----------------------------------------------------------------------------
/// \brief To differentiate log messages
enum class LogTypes : char
{
    DEBUG, //For regular debug messages
    WARNING, //For non-critical errors
    ERROR //For critical errors
};

//-----------------------------------------------------------------------------
// LogMessage
//-----------------------------------------------------------------------------
/// \brief Struct representing a log message with its type
struct LogMessage
{
    LogTypes type = LogTypes::DEBUG;
    std::string msg;

    explicit LogMessage(const std::string& log)
            : msg(log)
    {}

    explicit LogMessage(const LogTypes& logType, const std::string& log)
            : type(logType), msg(log)
    {}
};

//-----------------------------------------------------------------------------
// LogManagerInterface
//-----------------------------------------------------------------------------
/// \brief Used for the service locator
class LogManagerInterface
{
public:
    /**
     * \brief Generate a log message.
     * @param logType the type of the log message
     * @param log the log message
     */
    virtual void Log(LogTypes logType, const std::string& log) = 0;

    /**
     * \brief Retrieves the log history
     */
    virtual const std::vector<std::string>& GetLogs() = 0;
};

//-----------------------------------------------------------------------------
// NullLogManager
//-----------------------------------------------------------------------------
/// \brief Used for the service locator
class NullLogManager final : public LogManagerInterface
{
public:
    void Log([[maybe_unused]]LogTypes logType, [[maybe_unused]]const std::string& log) override
    {};

    const std::vector<std::string>& GetLogs() override
    {
        std::cerr << "Impossible to get log history from a null LogManager\n";
        assert(false);
    };
};

//-----------------------------------------------------------------------------
// LogManager
//-----------------------------------------------------------------------------
/// \brief Creates and stores log messages
class LogManager : public LogManagerInterface
{
public:
    ~LogManager() = default;

    void Log(LogTypes logType, const std::string& log) override;

    const std::vector<std::string>& GetLogs() override
    { return logHistory_; }

    void WriteToFile();

    void Close();

private:
    std::vector<std::string> logHistory_;

    std::mutex logMutex_;
    std::queue<std::function<void()>> tasks_;
    std::unique_ptr<std::thread> logThread_;
    std::condition_variable itemInQueue_;
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
 * \brief Generate a warning type log message
 */
void LogWarning(const std::string& msg);

/**
 * \brief Generate an error type log message
 */
void LogError(const std::string& msg);

/**
 * \brief Retrieves the log history
 */
const std::vector<std::string>& GetLogs();
}