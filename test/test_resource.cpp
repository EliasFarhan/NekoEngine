#include <gtest/gtest.h>
#include <engine/resource.h>

#include "sole.hpp"
#include <utilities/file_utility.h>

TEST(Engine, TestResource)
{
    std::string path = "./../../data/test/test.txt";
    std::string test = neko::LoadFile(path);
    neko::ResourceManager resourceManager;
    neko::ResourceId resourceId = resourceManager.LoadResource(path);
    int count = 0;
    while (!resourceManager.IsResourceReady(resourceId)) 
    {
        count++;
    }
    EXPECT_TRUE(std::string(resourceManager.GetResource(resourceId).dataBuffer) == test);
    EXPECT_FALSE(resourceManager.IsResourceReady(sole::uuid0()));
    resourceManager.Destroy();
}