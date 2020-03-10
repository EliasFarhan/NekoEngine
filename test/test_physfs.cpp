//
// Created by frousties on 3/9/20.
//

#include <gtest/gtest.h>
#include <engine/log.h>
#include <utilities/file_utility.h>
#include <utilities/physfs_utility.h>

#include <physfs.h>

namespace neko
{
TEST(IO, TestPhysFSTwoArchive)
{
    LogManager logger;
	
    physfs::InitPhysFs();
    const std::string filename = "sprites/wall.jpg";
	
    LogDebug(LogCategory::IO, "Mounting 'data1.zip'...");
    PHYSFS_mount("../../data/data1.zip", nullptr, 0);
    if (PHYSFS_exists(filename.c_str())) {
        LogDebug(LogCategory::IO, filename + " exists!");
    }
    else
    {
        LogError(LogCategory::IO, filename + " does NOT exist!");
    }

    LogDebug(LogCategory::IO, "Mounting 'data2.zip'...");
    PHYSFS_mount("../../data/data2.zip", nullptr, 0);
    if (PHYSFS_exists(filename.c_str())) {
        LogDebug(LogCategory::IO, filename + " exists!");
    }
    else
    {
        LogError(LogCategory::IO, filename + " does NOT exist!");
    }
	
    PHYSFS_setWriteDir("/");
    physfs::WriteStringToFile("test.txt", "hello");
}
}
