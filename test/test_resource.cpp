#include <gtest/gtest.h>
#include <engine/resource.h>

#include "sole.hpp"
#include "utilities/file_utility.h"

TEST(Engine, TestResource)
{
    std::string path = "./../../data/test/test.txt";
    std::string test = neko::LoadFile(path);
    neko::ResourceManager resourceManager;
    neko::ResourceId resourceId = resourceManager.LoadResource(path);
    int count = 0;
    while (!resourceManager.IsResourceReady(resourceId)) {
        count++;
    }
    //std::cout << count << "   " << resourceManager.GetResource(resourceId);
    EXPECT_TRUE(resourceManager.GetResource(resourceId) == test);
    EXPECT_FALSE(resourceManager.IsResourceReady(sole::uuid0()));
    EXPECT_FALSE(resourceManager.GetResource(sole::uuid0()) == test);
    resourceManager.Destroy();
}