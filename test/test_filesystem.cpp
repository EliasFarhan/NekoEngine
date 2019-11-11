//
// Created by efarhan on 11/11/19.
//

#include "utilities/file_utility.h"
#include <gtest/gtest.h>

TEST(Engine, TestFilesystem)
{
    std::string path = "../data/scenes/test.scene";
    std::string relative = "../data";
    std::cout << neko::GetRelativePath(path, relative) << "\n"; 
}