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
// LogManager definitions
//-----------------------------------------------------------------------------
LogManager::LogManager()
        : isRunning_(false)
{
    logThread_ = std::make_unique<std::thread>([this] {
        isRunning_ = true;
        this->Start();
    });
}

void LogManager::Start()
{
    std::unique_lock<std::mutex> lock(logMutex_);
    do
    {
        while (isRunning_ && tasks_.empty())
        { itemInQueue_.wait(lock); }

        while (!tasks_.empty())
        {
            const auto task = tasks_.front();
            tasks_.pop();
            lock.unlock();
            task();
            lock.lock();
        }
        itemInQueue_.notify_all();
    } while (isRunning_);
    itemInQueue_.notify_all();
}

void LogManager::Wait()
{
    std::unique_lock<std::mutex> lock(logMutex_);
    while (!tasks_.empty())
        itemInQueue_.wait(lock);
}

void LogManager::Close()
{
    Wait();

    {
        std::lock_guard<std::mutex> l(logMutex_);
        isRunning_ = false;
        itemInQueue_.notify_one();
    }

    logThread_->join();
}

void LogManager::Log(LogTypes logType, const std::string& log)
{
    std::unique_lock<std::mutex> lock(logMutex_);
    auto task([this, logType, log] {
        std::string message = "";

        auto curTime = time(nullptr);
        auto localTime = localtime(&curTime);
        message += '[' + std::to_string(localTime->tm_hour) + ':'
                   + std::to_string(localTime->tm_min) + ':'
                   + std::to_string(localTime->tm_sec) + "] ";

        switch (logType)
        {
            case LogTypes::DEBUG:
                message += "[DEBUG] ";
                break;
            case LogTypes::WARNING:
                message += "[WARNING] ";
                break;
            case LogTypes::PROBLEM:
                message += "[ERROR] ";
                break;
        }

        message += log + '\n';

        std::cout << message;
        logHistory_.push_back(message);
    });

    tasks_.push(task);
    itemInQueue_.notify_one();
    lock.unlock();
}

void LogManager::WriteToFile()
{
    std::unique_lock<std::mutex> lock(logMutex_);

    auto task([this] {
        auto curTime = time(nullptr);
        auto localTime = localtime(&curTime);

        const std::string filePath = "../data/logs/";
        std::ostringstream ss;
        ss << std::setw(2) << std::setfill('0') << localTime->tm_mday << "-"
           << std::setw(2) << std::setfill('0') << localTime->tm_mon + 1 << "-"
           << std::to_string(localTime->tm_year + 1900) << "_"
           << std::setw(2) << std::setfill('0') << localTime->tm_hour << "-"
           << std::setw(2) << std::setfill('0') << localTime->tm_min << "-"
           << std::setw(2) << std::setfill('0') << localTime->tm_sec;

        const std::string fileName(ss.str());

        std::string fileContent = "/--------------------------------------------------------------------------------\\\n";
        fileContent += "|                                 NekoEngine logs                                |\n";
        fileContent += "|                              " + fileName + "                               |\n";
        fileContent += "|              Copyright (c) 2017-2020 SAE Institute Switzerland AG              |\n";
        fileContent += "\\--------------------------------------------------------------------------------/\n\n";

        fileContent += "Program start (=^ ◡ ^=)\n";
        fileContent += "--------------------------------------------------------------------------------\n";
        for (auto& line : logHistory_)
        {
            fileContent += line;
        }

        CreateDirectory(filePath);
        WriteStringToFile(filePath + fileName + ".log", fileContent);
    });

    tasks_.push(task);
    itemInQueue_.notify_one();
    lock.unlock();
}

//-----------------------------------------------------------------------------
// Shorthands definitions
//-----------------------------------------------------------------------------
void LogDebug(const std::string& msg)
{
    Log::get().Log(LogTypes::DEBUG, msg);
}

void LogWarning(const std::string& msg)
{
    Log::get().Log(LogTypes::WARNING, msg);
}

void LogError(const std::string& msg)
{
    Log::get().Log(LogTypes::PROBLEM, msg);
}

const std::vector<std::string>& GetLogs()
{
    return Log::get().GetLogs();
}
}
