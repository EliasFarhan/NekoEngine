/*
MIT License

Copyright (c) 2019 SAE Institute Switzerland AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <iostream>
#include <fstream>
#include <xxhash.hpp>
#include <sole.hpp>
#include <gtest/gtest.h>
#include "utilities/file_utility.h"
#include "engine/engine.h"

TEST(Engine, TestUUIDToStringToUUID)
{
	const sole::uuid id = sole::uuid0();
	const auto idStr = id.str();
	const auto convertId = sole::rebuild(idStr);
	EXPECT_TRUE(id == convertId);
}

TEST(Engine, TestAssetImport)
{
	neko::Configuration config;
	config.dataRootPath = "../";
	std::vector<std::string> filenames =
	{
		config.dataRootPath+"data/sprites/platform.jpg",
		config.dataRootPath+"data/sprites/wall.jpg",
		config.dataRootPath+"data/sprites/icons/icons8-road-48.png",
		config.dataRootPath+"fake/path/file.png",
		config.dataRootPath+"other/fake/path/file.png",
	};
	std::vector<xxh::hash_t<64>> fileHashes;
	fileHashes.reserve(filenames.size());
	for (auto& filename : filenames)
	{
		xxh::hash_state_t<64> hash_stream(0);
		hash_stream.update(filename);
		neko::BufferFile bufferFile;
		bufferFile.Load(filename);
		if (bufferFile.dataBuffer != nullptr)
		{
			hash_stream.update(bufferFile.dataBuffer, bufferFile.dataLength);
		}
		bufferFile.Destroy();
		const xxh::hash_t<64> final_hash = hash_stream.digest();
		fileHashes.push_back(final_hash);
	}
	EXPECT_NE(fileHashes[0], fileHashes[1]);
	EXPECT_NE(fileHashes[1], fileHashes[2]);
	EXPECT_NE(fileHashes[2], fileHashes[0]);
}