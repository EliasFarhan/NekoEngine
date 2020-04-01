#include <gtest/gtest.h>
#include <engine/resource.h>

#include <sole.hpp>
#include <utilities/file_utility.h>
#ifdef EASY_PROFILE_USE
#include <easy/profiler.h>
#endif

#include "engine/engine.h"

TEST(Engine, TestResource)
{
#ifdef EASY_PROFILE_USE
    profiler::startListen();
    EASY_MAIN_THREAD;
    EASY_PROFILER_ENABLE;
    EASY_BLOCK("TestResource");
#endif

    const sole::uuid randomUuid = sole::uuid4();
    neko::Configuration config;
    std::string path = config.dataRootPath + "data/test/test.txt";
    std::string fileContent = neko::LoadFile(path);
    neko::ResourceManager resourceManager;
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("InitResource");
#endif
    resourceManager.Init();
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
    EASY_BLOCK("LoadResource");
#endif
    neko::ResourceId resourceId = resourceManager.LoadResource(path);
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    int count = 0;
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("WaitingResource");
#endif
    while (!resourceManager.IsResourceReady(resourceId))
    {
        //count++;
    }
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;

    EASY_BLOCK("EXPECT_TRUE");
#endif
    EXPECT_TRUE(resourceManager.GetResource(resourceId) == fileContent);
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
    EASY_BLOCK("EXPECT_FALSE");
    EASY_BLOCK("Generate UUID", profiler::colors::Red200);
    EASY_END_BLOCK;
#endif
    EXPECT_FALSE(resourceManager.IsResourceReady(randomUuid));
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    resourceManager.Destroy();
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
    profiler::dumpBlocksToFile("resource_profile.prof");
#endif
}