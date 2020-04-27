#include <gtest/gtest.h>
#include <engine/resource.h>

#include <sole.hpp>
#include <utilities/file_utility.h>
#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#include <easy/arbitrary_value.h> // EASY_VALUE, EASY_ARRAY are defined here
#endif

#include <queue>

#include "engine/assert.h"
#include "engine/engine.h"


TEST(Resource, TestResourceSmall)
{
    const sole::uuid test = sole::uuid4();
#ifdef EASY_PROFILE_USE
    EASY_PROFILER_ENABLE;
    EASY_BLOCK("TestResourceSmall", profiler::colors::Gold);
#endif
    neko::BufferFile fileContent;
    const size_t nbTest = 100;
    const sole::uuid randomUuid = sole::uuid4();
    neko::Configuration config;
    std::string path = config.dataRootPath + "data/test/smallFile.txt";
    std::vector<neko::ResourceId> resourcesId = std::vector<neko::ResourceId>();
    neko::ResourceManager resourceManager;
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Init ResourceManager");
#endif
    resourceManager.Init();
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    for (size_t i = 0; i < nbTest; i++)
    {
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("Call LoadResource", profiler::colors::Gold);
#endif
        resourcesId.push_back(resourceManager.LoadResource(path));
#ifdef EASY_PROFILE_USE
        EASY_END_BLOCK;
#endif
    }
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Do some stuff", profiler::colors::Gold);
#endif
    for (int i = 0; i < nbTest; i++)
    {
        fileContent.Load(path);
        float isqrt = sqrt(i);
    }
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("WaitingResource", profiler::colors::Gold);
#endif
    while (!resourceManager.IsResourceReady(resourcesId[nbTest - 1]))
    {
    }
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    for (int i = 0; i < nbTest; i++)
    {
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("EXPECT_TRUE", profiler::colors::Gold);
#endif
        EXPECT_TRUE(resourceManager.GetResource(resourcesId[i]) == fileContent);
#ifdef EASY_PROFILE_USE
        EASY_END_BLOCK;
#endif
    }
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("EXPECT_FALSE", profiler::colors::Gold);
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
    //profiler::dumpBlocksToFile("resourcesmall_profile.prof");
#endif
}


TEST(Resource, TestResourceBig)
{
    const sole::uuid test = sole::uuid4();
#ifdef EASY_PROFILE_USE
    EASY_PROFILER_ENABLE;
    EASY_BLOCK("TestResourceBig", profiler::colors::Gold);
#endif
    neko::BufferFile fileContent;
    const size_t nbTest = 10;
    const sole::uuid randomUuid = sole::uuid4();
    neko::Configuration config;
    std::string path = config.dataRootPath + "data/test/bigFile.json";
    std::vector<neko::ResourceId> resourcesId = std::vector<neko::ResourceId>(nbTest, randomUuid);
    neko::ResourceManager resourceManager;
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("InitResource");
#endif
    resourceManager.Init();
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    for (int i = 0; i < nbTest; i++)
    {
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("Call LoadResource");
#endif
        resourcesId[i] = resourceManager.LoadResource(path);
#ifdef EASY_PROFILE_USE
        EASY_END_BLOCK;
#endif
    }
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Do some stuff", profiler::colors::Gold);
#endif
    for (int i = 0; i < nbTest; i++)
    {
        fileContent.Load(path);
        float isqrt = sqrt(i);
    }
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("WaitingResource");
#endif
    while (!resourceManager.IsResourceReady(resourcesId[nbTest - 1]))
    {
        //count++;
    }
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    for (int i = 0; i < nbTest; i++)
    {
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("EXPECT_TRUE");
#endif
        EXPECT_TRUE(resourceManager.GetResource(resourcesId[i]) == fileContent);
#ifdef EASY_PROFILE_USE
        EASY_END_BLOCK;
#endif
    }
#ifdef EASY_PROFILE_USE
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
    //profiler::dumpBlocksToFile("resourcebig_profile.prof");
#endif
}


TEST(Resource, TestSoleUUID)
{
#ifdef EASY_PROFILE_USE
    EASY_PROFILER_ENABLE;
    EASY_BLOCK("TestSoleUUID", profiler::colors::Gold);
#endif
    const size_t nbTest = 10000;
    for (int i = 0; i < nbTest; i++)
    {
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("Create a uuid4");
#endif
        const sole::uuid randomUuid = sole::uuid4();
#ifdef EASY_PROFILE_USE
        EASY_END_BLOCK;
#endif
    }
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
    profiler::dumpBlocksToFile("uuid_profile.prof");
#endif
}


TEST(Resource, PhysFSLoad)
{
    const sole::uuid test = sole::uuid4();
    neko::BufferFile fileContent;
    const size_t nbTest = 1;
    const sole::uuid randomUuid = sole::uuid4();
    neko::Configuration config;
    std::string path = "test.txt";
    std::string archivedPath = config.dataRootPath + "data/data2.zip";
    neko::ResourceId resourcesId;
    neko::ResourceManager resourceManager;
    resourceManager.Init();
    resourcesId = resourceManager.LoadArchivedResource(archivedPath, path);
    fileContent.LoadFromArchived(archivedPath, path);
    while (!resourceManager.IsResourceReady(resourcesId))
    {
    }
    EXPECT_TRUE(resourceManager.GetResource(resourcesId) == fileContent);
    EXPECT_FALSE(resourceManager.IsResourceReady(randomUuid));
    resourceManager.Destroy();
}
