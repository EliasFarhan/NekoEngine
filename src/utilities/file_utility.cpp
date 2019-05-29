/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

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
#include <utilities/file_utility.h>
#include <sstream>
#include <functional>
#include "engine/log.h"

#ifdef __APPLE__

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

#ifdef WIN32
#include <filesystem>
#endif

#ifdef __linux__
#include <filesystem>
namespace fs = std::filesystem;
#endif

namespace neko
{
bool FileExists(const std::string & filename)
{
	const fs::path p = filename;
	return fs::exists(p);
}
bool IsRegularFile(std::string& filename)
{
	const fs::path p = filename;
    return fs::is_regular_file(p);

}
bool IsDirectory(std::string & filename)
{
	const fs::path p = filename;
	return fs::is_directory(p);
}
void IterateDirectory(std::string & dirname, std::function<void(std::string)> func)
{
	
	if (IsDirectory(dirname))
	{
		for (auto& p : fs::directory_iterator(dirname))
		{
			func(p.path().generic_string());
		}
	}
}

std::ifstream::pos_type CalculateFileSize(const std::string& filename)
{
	std::ifstream in(filename, std::ifstream::binary | std::ifstream::ate);
	return in.tellg();
}
bool CreateDirectory(const std::string& dirname)
{
	return fs::create_directory(dirname);
}
bool RemoveDirectory(const std::string &dirname, bool removeAll)
{
	if(removeAll)
	{
		return fs::remove_all(dirname);
	} 
	else
	{
		return fs::remove(dirname);
	}
}

const std::string LoadFile(std::string path)
{
	std::ifstream t(path);
	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	return str;
}

std::string GetFilenameExtension(std::string path)
{
	std::string extension = "";
	const auto folderLastIndex = path.find_last_of('/');
	std::string filename = path.substr(folderLastIndex + 1, path.size());
	const auto filenameExtensionIndex = filename.find_last_of('.');
	if (filenameExtensionIndex > path.size())
	{
		std::ostringstream oss;
		oss << "[Error] Path: " << path << " has not a correct extension";
		logDebug(oss.str());
		return extension;
	}
	extension = filename.substr(filenameExtensionIndex);
	return extension;
}
}
