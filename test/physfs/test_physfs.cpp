//
// Created by frousties on 3/9/20.
//

#include <gtest/gtest.h>
#include <engine/log.h>
#include <engine/prefab.h>
#include <utilities/file_utility.h>
#include <physfs_utility.h>

#include <engine/engine.h>

namespace neko
{
TEST(PhysFS, TestTwoArchive)
{
    LogManager logger;

    const Configuration config;
	
    physfs::InitPhysFs();
    const std::string_view filename = "sprites/wall.jpg";
	
    EXPECT_TRUE(physfs::OpenArchive(config.dataRootPath + "data/data1.zip"));
    EXPECT_FALSE(physfs::FileExists(filename));

    EXPECT_TRUE(physfs::OpenArchive(config.dataRootPath + "data/data2.zip"));
    EXPECT_TRUE(physfs::FileExists(filename));
	
    logger.Wait();
}

TEST(PhysFS, TestOpenFile)
{
    LogManager logger;
    physfs::InitPhysFs();
	
    const char* output = physfs::ReadFile("test.txt");
    EXPECT_NE(output, nullptr);
	if (output != nullptr) std::cout << output << '\n';
}
}
