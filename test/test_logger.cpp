//
// Created by frousties on 11/02/2020.
//

#include <gtest/gtest.h>
#include <engine/log.h>

namespace neko
{
TEST(Logs, TestLogsBasic)
{
    LogManager logger;

    LogDebug(LogCategory::ENGINE, "Engine is running");
    LogWarning("That feature isn't fully supported yet");
    LogError("Could not retrieve value!");

    logger.Wait();
}

TEST(Logs, TestLogsFromNewThread)
{
    LogManager logger;

    auto task([]
    {
        LogDebug("Task Begin");
        LogDebug("This is a task");
    });
	
    auto newThread = std::thread([task]
    {
        LogDebug(LogCategory::ENGINE, "Creating logs from another thread");
        task();
    });

    newThread.join();

    logger.Wait();
}
}
