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
    EASY_BLOCK("Loading", profiler::colors::Gold);
#endif
    fileContent.Load(path);
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
    profiler::dumpBlocksToFile("resourcesmall_profile.prof");
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
    EASY_BLOCK("Loading");
#endif
    fileContent.Load(path);
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
    profiler::dumpBlocksToFile("resourcebig_profile.prof");
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
//
//TEST(Resource, TestResource2)
//{
//#ifdef EASY_PROFILE_USE
//    EASY_PROFILER_ENABLE;
//    EASY_BLOCK("TestResource2", profiler::colors::Gold);
//#endif
//    neko::BufferFile fileContent;
//    const size_t nbTest = 10;
//    const sole::uuid randomUuid = sole::uuid4();
//    neko::Configuration config;
//    std::string path = config.dataRootPath + "data/test/test.txt";
//    std::string path1 = config.dataRootPath + "data/test/LevelConcept.pokscene";
//    std::vector<neko::ResourceId> resourcesId = std::vector<neko::ResourceId>(nbTest, randomUuid);
//    neko::ResourceManager2 resourceManager;
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("InitResource");
//#endif
//    resourceManager.Init();
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    fileContent.Load(path);
//    for (int i = 0; i < nbTest; i++)
//    {
//#ifdef EASY_PROFILE_USE
//        EASY_BLOCK("Call LoadResource");
//#endif
//        resourcesId[i] = resourceManager.LoadResource(path);
//#ifdef EASY_PROFILE_USE
//        EASY_END_BLOCK;
//#endif
//    }/*
//    for (int i = nbTest / 2; i < nbTest; i++)
//    {
//        fileContent.Load(path1);
//#ifdef EASY_PROFILE_USE
//        EASY_BLOCK("Call LoadResource");
//#endif
//        resourcesId[i] = resourceManager.LoadResource(path);
//#ifdef EASY_PROFILE_USE
//        EASY_END_BLOCK;
//#endif
//    }*/
//    //fileContent.Load(path);
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("Loading");
//#endif
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("WaitingResource");
//#endif
//    while (!resourceManager.IsResourceReady(resourcesId[nbTest - 1]))
//    {
//        //count++;
//    }
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    for (int i = 0; i < nbTest; i++)
//    {
//#ifdef EASY_PROFILE_USE
//        EASY_BLOCK("EXPECT_TRUE");
//#endif
//        EXPECT_TRUE(resourceManager.GetResource(resourcesId[i]) == std::string(fileContent.dataBuffer));
//#ifdef EASY_PROFILE_USE
//        EASY_END_BLOCK;
//#endif
//    }
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("EXPECT_FALSE");
//    EASY_BLOCK("Generate UUID", profiler::colors::Red200);
//    EASY_END_BLOCK;
//#endif
//    EXPECT_FALSE(resourceManager.IsResourceReady(randomUuid));
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    resourceManager.Destroy();
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//    //profiler::dumpBlocksToFile("resource2_profile.prof");
//#endif
//}
//
//
//TEST(Resource, TestResource3)
//{
//#ifdef EASY_PROFILE_USE
//    EASY_PROFILER_ENABLE;
//    EASY_BLOCK("TestResource3", profiler::colors::Gold);
//#endif
//    neko::BufferFile fileContent;
//    const size_t nbTest = 10;
//    const sole::uuid randomUuid = sole::uuid4();
//    neko::Configuration config;
//    std::string path = config.dataRootPath + "data/test/test.txt";
//    std::string path1 = config.dataRootPath + "data/test/LevelConcept.pokscene";
//    std::vector<neko::ResourceId> resourcesId = std::vector<neko::ResourceId>(nbTest, randomUuid);
//    neko::ResourceManager3 resourceManager;
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("InitResource");
//#endif
//    resourceManager.Init();
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    for (int i = 0; i < nbTest; i++)
//    {
//        fileContent.Load(path);
//#ifdef EASY_PROFILE_USE
//        EASY_BLOCK("Call LoadResource");
//#endif
//        resourcesId[i] = resourceManager.LoadResource(path);
//#ifdef EASY_PROFILE_USE
//        EASY_END_BLOCK;
//#endif
//    }/*
//    for (int i = nbTest / 2; i < nbTest; i++)
//    {
//        fileContent.Load(path1);
//#ifdef EASY_PROFILE_USE
//        EASY_BLOCK("Call LoadResource");
//#endif
//        resourcesId[i] = resourceManager.LoadResource(path);
//#ifdef EASY_PROFILE_USE
//        EASY_END_BLOCK;
//#endif
//    }*/
//    //fileContent.Load(path);
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("Loading");
//#endif
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("WaitingResource");
//#endif
//    while (!resourceManager.IsResourceReady(resourcesId[nbTest - 1]))
//    {
//        //count++;
//    }
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    for (int i = 0; i < nbTest; i++)
//    {
//#ifdef EASY_PROFILE_USE
//        EASY_BLOCK("EXPECT_TRUE");
//#endif
//        EXPECT_TRUE(resourceManager.GetResource(resourcesId[i]) == fileContent);
//#ifdef EASY_PROFILE_USE
//        EASY_END_BLOCK;
//#endif
//    }
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("EXPECT_FALSE");
//    EASY_BLOCK("Generate UUID", profiler::colors::Red200);
//    EASY_END_BLOCK;
//#endif
//    EXPECT_FALSE(resourceManager.IsResourceReady(randomUuid));
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    resourceManager.Destroy();
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//    profiler::dumpBlocksToFile("resource3_profile.prof");
//#endif
//}
//
//
//TEST(Resource, TestResource1)
//{
//    std::promise<int> p;
//#ifdef EASY_PROFILE_USE
//    EASY_PROFILER_ENABLE;
//    EASY_BLOCK("TestResource1", profiler::colors::Gold);
//#endif
//    neko::BufferFile fileContent;
//    const size_t nbTest = 10;
//    const sole::uuid randomUuid = sole::uuid4();
//    neko::Configuration config;
//    std::string path = config.dataRootPath + "data/test/test.txt";
//    std::string path1 = config.dataRootPath + "data/test/LevelConcept.pokscene";
//    std::vector<neko::ResourceId> resourcesId = std::vector<neko::ResourceId>(nbTest, randomUuid);
//    neko::ResourceManager1 resourceManager;
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("InitResource");
//#endif
//    resourceManager.Init();
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    for (int i = 0; i < nbTest ; i++)
//    {
//        fileContent.Load(path);
//#ifdef EASY_PROFILE_USE
//        EASY_BLOCK("Call LoadResource");
//#endif
//        resourcesId[i] = resourceManager.LoadResource(path);
//#ifdef EASY_PROFILE_USE
//        EASY_END_BLOCK;
//#endif
//    }
////    for (int i = nbTest / 2; i < nbTest; i++)
////    {
////        fileContent.Load(path1);
////#ifdef EASY_PROFILE_USE
////        EASY_BLOCK("Call LoadResource");
////#endif
////        resourcesId[i] = resourceManager.LoadResource(path);
////#ifdef EASY_PROFILE_USE
////        EASY_END_BLOCK;
////#endif
////    }
//    //fileContent.Load(path);
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("Loading");
//#endif
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("WaitingResource");
//#endif
//    while (!resourceManager.IsResourceReady(resourcesId[nbTest - 1]))
//    {
//        //count++;
//    }
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    for (int i = 0; i < nbTest; i++)
//    {
//#ifdef EASY_PROFILE_USE
//        EASY_BLOCK("EXPECT_TRUE");
//#endif
//        EXPECT_TRUE(resourceManager.GetResource(resourcesId[i]) == fileContent);
//#ifdef EASY_PROFILE_USE
//        EASY_END_BLOCK;
//#endif
//    }
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("EXPECT_FALSE");
//    EASY_BLOCK("Generate UUID", profiler::colors::Red200);
//    EASY_END_BLOCK;
//#endif
//    EXPECT_FALSE(resourceManager.IsResourceReady(randomUuid));
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    resourceManager.Destroy();
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//    //profiler::dumpBlocksToFile("resource1_profile.prof");
//#endif
//}
//
//
//TEST(Resource, TestResource)
//{
//    const sole::uuid test = sole::uuid4();
//#ifdef EASY_PROFILE_USE
//    EASY_PROFILER_ENABLE;
//    EASY_BLOCK("TestResource", profiler::colors::Gold);
//#endif
//    neko::BufferFile fileContent;
//    const size_t nbTest = 10;
//    const sole::uuid randomUuid = sole::uuid4();
//    neko::Configuration config;
//    std::string path = config.dataRootPath + "data/test/test.txt";
//    std::string path1 = config.dataRootPath + "data/test/LevelConcept.pokscene";
//    std::vector<neko::ResourceId> resourcesId = std::vector<neko::ResourceId>(nbTest, randomUuid);
//    neko::ResourceManager resourceManager;
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("InitResource");
//#endif
//    resourceManager.Init();
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    for (int i = 0; i < nbTest; i++)
//    {
//        fileContent.Load(path);
//#ifdef EASY_PROFILE_USE
//        EASY_BLOCK("Call LoadResource");
//#endif
//        resourcesId[i] = resourceManager.LoadResource(path);
//#ifdef EASY_PROFILE_USE
//        EASY_END_BLOCK;
//#endif
//    }/*
//    for (int i = nbTest/2; i < nbTest; i++)
//    {
//        fileContent.Load(path1);
//#ifdef EASY_PROFILE_USE
//        EASY_BLOCK("Call LoadResource");
//#endif
//        resourcesId[i] = resourceManager.LoadResource(path);
//#ifdef EASY_PROFILE_USE
//        EASY_END_BLOCK;
//#endif
//    }*/
//    fileContent.Load(path);
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("Loading");
//#endif
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("WaitingResource");
//#endif
//    while (!resourceManager.IsResourceReady(resourcesId[nbTest - 1]))
//    {
//        count++;
//    }
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    for (int i = 0; i < nbTest; i++)
//    {
//#ifdef EASY_PROFILE_USE
//        EASY_BLOCK("EXPECT_TRUE");
//#endif
//        EXPECT_TRUE(resourceManager.GetResource(resourcesId[i]) == fileContent);
//#ifdef EASY_PROFILE_USE
//        EASY_END_BLOCK;
//#endif
//    }
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("EXPECT_FALSE");
//    EASY_BLOCK("Generate UUID", profiler::colors::Red200);
//    EASY_END_BLOCK;
//#endif
//    EXPECT_FALSE(resourceManager.IsResourceReady(randomUuid));
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    resourceManager.Destroy();
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//    profiler::dumpBlocksToFile("resource_profile.prof");
//#endif
//}
//
//TEST(Resource, TestResource2)
//{
//#ifdef EASY_PROFILE_USE
//    EASY_PROFILER_ENABLE;
//    EASY_BLOCK("TestResource2", profiler::colors::Gold);
//#endif
//    neko::BufferFile fileContent;
//    const size_t nbTest = 10;
//    const sole::uuid randomUuid = sole::uuid4();
//    neko::Configuration config;
//    std::string path = config.dataRootPath + "data/test/test.txt";
//    std::string path1 = config.dataRootPath + "data/test/LevelConcept.pokscene";
//    std::vector<neko::ResourceId> resourcesId = std::vector<neko::ResourceId>(nbTest, randomUuid);
//    neko::ResourceManager2 resourceManager;
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("InitResource");
//#endif
//    resourceManager.Init();
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    fileContent.Load(path);
//    for (int i = 0; i < nbTest; i++)
//    {
//#ifdef EASY_PROFILE_USE
//        EASY_BLOCK("Call LoadResource");
//#endif
//        resourcesId[i] = resourceManager.LoadResource(path);
//#ifdef EASY_PROFILE_USE
//        EASY_END_BLOCK;
//#endif
//    }/*
//    for (int i = nbTest / 2; i < nbTest; i++)
//    {
//        fileContent.Load(path1);
//#ifdef EASY_PROFILE_USE
//        EASY_BLOCK("Call LoadResource");
//#endif
//        resourcesId[i] = resourceManager.LoadResource(path);
//#ifdef EASY_PROFILE_USE
//        EASY_END_BLOCK;
//#endif
//    }*/
//    //fileContent.Load(path);
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("Loading");
//#endif
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("WaitingResource");
//#endif
//    while (!resourceManager.IsResourceReady(resourcesId[nbTest - 1]))
//    {
//        //count++;
//    }
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    for (int i = 0; i < nbTest; i++)
//    {
//#ifdef EASY_PROFILE_USE
//        EASY_BLOCK("EXPECT_TRUE");
//#endif
//        EXPECT_TRUE(resourceManager.GetResource(resourcesId[i]) == std::string(fileContent.dataBuffer));
//#ifdef EASY_PROFILE_USE
//        EASY_END_BLOCK;
//#endif
//    }
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("EXPECT_FALSE");
//    EASY_BLOCK("Generate UUID", profiler::colors::Red200);
//    EASY_END_BLOCK;
//#endif
//    EXPECT_FALSE(resourceManager.IsResourceReady(randomUuid));
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    resourceManager.Destroy();
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//    //profiler::dumpBlocksToFile("resource2_profile.prof");
//#endif
//}
//
//
//TEST(Resource, TestResource3)
//{
//#ifdef EASY_PROFILE_USE
//    EASY_PROFILER_ENABLE;
//    EASY_BLOCK("TestResource3", profiler::colors::Gold);
//#endif
//    neko::BufferFile fileContent;
//    const size_t nbTest = 10;
//    const sole::uuid randomUuid = sole::uuid4();
//    neko::Configuration config;
//    std::string path = config.dataRootPath + "data/test/test.txt";
//    std::string path1 = config.dataRootPath + "data/test/LevelConcept.pokscene";
//    std::vector<neko::ResourceId> resourcesId = std::vector<neko::ResourceId>(nbTest, randomUuid);
//    neko::ResourceManager3 resourceManager;
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("InitResource");
//#endif
//    resourceManager.Init();
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    for (int i = 0; i < nbTest; i++)
//    {
//        fileContent.Load(path);
//#ifdef EASY_PROFILE_USE
//        EASY_BLOCK("Call LoadResource");
//#endif
//        resourcesId[i] = resourceManager.LoadResource(path);
//#ifdef EASY_PROFILE_USE
//        EASY_END_BLOCK;
//#endif
//    }/*
//    for (int i = nbTest / 2; i < nbTest; i++)
//    {
//        fileContent.Load(path1);
//#ifdef EASY_PROFILE_USE
//        EASY_BLOCK("Call LoadResource");
//#endif
//        resourcesId[i] = resourceManager.LoadResource(path);
//#ifdef EASY_PROFILE_USE
//        EASY_END_BLOCK;
//#endif
//    }*/
//    //fileContent.Load(path);
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("Loading");
//#endif
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("WaitingResource");
//#endif
//    while (!resourceManager.IsResourceReady(resourcesId[nbTest - 1]))
//    {
//        //count++;
//    }
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    for (int i = 0; i < nbTest; i++)
//    {
//#ifdef EASY_PROFILE_USE
//        EASY_BLOCK("EXPECT_TRUE");
//#endif
//        EXPECT_TRUE(resourceManager.GetResource(resourcesId[i]) == fileContent);
//#ifdef EASY_PROFILE_USE
//        EASY_END_BLOCK;
//#endif
//    }
//#ifdef EASY_PROFILE_USE
//    EASY_BLOCK("EXPECT_FALSE");
//    EASY_BLOCK("Generate UUID", profiler::colors::Red200);
//    EASY_END_BLOCK;
//#endif
//    EXPECT_FALSE(resourceManager.IsResourceReady(randomUuid));
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//#endif
//    resourceManager.Destroy();
//#ifdef EASY_PROFILE_USE
//    EASY_END_BLOCK;
//    profiler::dumpBlocksToFile("resource3_profile.prof");
//#endif
//}
