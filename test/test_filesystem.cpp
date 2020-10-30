//
// Created by efarhan on 11/11/19.
//

#include "utilities/file_utility.h"
#include <gtest/gtest.h>

#include <filesystem>
namespace fs = std::filesystem;

TEST(Engine, TestFilesystem)
{
    std::string path = "../data/scenes/test.scene";
    std::string relative = "../data";
    std::cout << neko::GetRelativePath(path, relative) << "\n"; 
}

TEST(Engine, TestPathCompare)
{
	const fs::path texture1 = "../data/sprites/wall.jpg";
	const fs::path texture2 = "../test/../data/sprites/wall.jpg";
	const fs::path texture3 = "../data/nanosuit2/body_dif.png";

	std::cout << "Texture1: "<< fs::absolute(texture1).generic_string()<<
		" texture2: "<< fs::absolute(texture2).generic_string()<<'\n';
	
    //EXPECT_TRUE(fs::absolute(texture1) == fs::absolute(texture2));
    EXPECT_TRUE(texture1 != texture3);
	
}