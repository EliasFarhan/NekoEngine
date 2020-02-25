//
// Created by frousties on 11/02/2020.
//

#include <gtest/gtest.h>
#include <engine/log.h>

TEST(Engine, TestLogs)
{
    using namespace neko;

    LogManager logger;
    Log::provide(&logger);

    LogDebug("Hello");
    LogWarning("How");
    LogError("are");
    LogDebug("you?");

    logger.WriteToFile();

    logger.Close();
}