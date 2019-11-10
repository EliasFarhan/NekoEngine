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

TEST(Engine, TestAssetImport)
{
	sole::uuid id = sole::uuid0();
	auto idStr = id.str();
	auto convertId = sole::rebuild(idStr);
	EXPECT_TRUE(id == convertId);
	std::vector<std::string> filenames{
		"data/sprites/platform.png",
		"data/editor/play.png",
		"data/editor/star.png",
		"fake/path/file.png",
		"other/fake/path/file.png",
	};
	for (auto& filename : filenames)
	{
		xxh::hash_state_t<64> hash_stream(0);
		hash_stream.update(filename);
		if (std::ifstream input{ filename , std::ios::binary })
		{
			input.unsetf(std::ios::skipws);
			input.seekg(0, std::ios::end);
			auto fileSize = input.tellg();
			input.seekg(0, std::ios::beg);
			std::vector<char> content;
			content.reserve(fileSize);

			// read the data:
			content.insert(content.begin(),
				std::istream_iterator<char>(input),
				std::istream_iterator<char>());
			hash_stream.update(content);
		}
		else
		{
			std::cerr << "Could not open filename: " << filename << "\n";
		}
		const xxh::hash_t<64> final_hash = hash_stream.digest();
		std::cout << "Filename: " << filename << " Hash: " << final_hash << "\n";
	}
}