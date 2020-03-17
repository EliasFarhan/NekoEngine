//
// Created by frousties on 3/9/20.
//

#include <gtest/gtest.h>
#include <engine/log.h>
#include <engine/prefab.h>
#include <utilities/file_utility.h>
#include <physfs_utility.h>

namespace neko
{
TEST(PhysFS, TestPhysFSTwoArchive)
{
    LogManager logger;
	
    physfs::InitPhysFs();
    const std::string_view filename = "sprites/wall.jpg";
	
    physfs::OpenArchive("../../data/data1.zip");
    physfs::FileExists(filename);

    physfs::OpenArchive("../../data/data2.zip");
    physfs::FileExists(filename);

    const auto output = physfs::ReadFile("test.txt");
    std::cout << output << '\n';

    logger.Wait();
}
}