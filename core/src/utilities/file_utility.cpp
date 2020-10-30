/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

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

#include <fmt/format.h>


#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif

#if defined(__ANDROID__)
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>


static AAssetManager* assetManager = nullptr;

extern "C"
{
JNIEXPORT void JNICALL
Java_swiss_sae_gpr5300_MainActivity_load(JNIEnv *env, [[maybe_unused]] jclass clazz, jobject mgr) {
	assetManager = AAssetManager_fromJava(env, mgr);
	logDebug("Asset Manager from JNI loaded!");
}
}
namespace neko
{
ResourceJob::ResourceJob() : Job([this]{bufferFile_.Load(filePath_);})
{
}
void ResourceJob::SetFilePath(std::string_view path)
{
    filePath_ = path;
}

bool FileExists(const std::string_view path)
{
	{
		std::ostringstream oss;
		oss << "Checking file exists: " << path;
		logDebug(oss.str());
	}
	AAsset* file = AAssetManager_open(assetManager, path.data(), AASSET_MODE_STREAMING);
	bool exist = file != nullptr;
	if (exist)
		AAsset_close(file);
	return exist;
}
std::string GetFilenameExtension(const std::string_view path)
{
	const std::string extension = path.data();
	return extension.substr(extension.find_last_of("."));
}
std::string GetCurrentPath()
{
	return "";
}
const std::string LoadFile(const std::string& path)
{
	AAsset* file = AAssetManager_open(assetManager, path.c_str(), AASSET_MODE_BUFFER);
	if (file == nullptr)
		return "";
	// Get the file length
	const  size_t fileLength = static_cast<const size_t>(AAsset_getLength64(file));

	// Allocate memory to read your file
	char* fileContent = new char[fileLength + 1];

	// Read your file
	AAsset_read(file, fileContent, fileLength);
	fileContent[fileLength] = '\0';
	AAsset_close(file);
	std::string str(fileContent);
	delete[] fileContent;
	return str;
}


void BufferFile::Load(std::string_view path)
{
	AAsset* file = AAssetManager_open(assetManager, path.data(), AASSET_MODE_BUFFER);
	if (file == nullptr)
		return;
	// Get the file length
	dataLength = static_cast<const size_t>(AAsset_getLength64(file));

	// Allocate memory to read your file
	dataBuffer = new unsigned char[dataLength + 1];
	dataBuffer[dataLength] = '\0';

	// Read your file
	AAsset_read(file, dataBuffer, dataLength);
	AAsset_close(file);
}

void BufferFile::Destroy()
{
	delete[] dataBuffer;
	dataBuffer = nullptr;
	dataLength = 0;
}


BufferFile::~BufferFile()
{
	Destroy();
}

void ResourceJob::Reset()
{
	Job::Reset();
	bufferFile_.Destroy();
}
}

#else

#ifdef __APPLE__

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

#ifdef WIN32
#include <filesystem>
namespace fs = std::filesystem;
#endif

#if defined(__linux__) || defined (EMSCRIPTEN)
#include <filesystem>
namespace fs = std::filesystem;
#endif

namespace neko
{
ResourceJob::ResourceJob() : Job([this]
{
#ifdef EASY_PROFILE_USE
		EASY_BLOCK("Load Resource");
#endif
    bufferFile_.Destroy();
    bufferFile_.Load(filePath_);
})
{
}

void ResourceJob::SetFilePath(std::string_view path)
{
    filePath_ = path;
}

void ResourceJob::Reset()
{
    Job::Reset();
    bufferFile_.Destroy();
}


void BufferFile::Load(std::string_view path)
{
    if(dataBuffer != nullptr)
    {
        Destroy();
    }
    std::ifstream is(path.data(),std::ifstream::binary);
    if(!is)
    {
        logDebug(fmt::format("[Error] Could not open file: {}  for BufferFile", path));
        dataLength = 0;
        dataBuffer = nullptr;
    }
    else
    {
        is.seekg(0, is.end);
        dataLength = is.tellg();
        is.seekg(0, is.beg);
        dataBuffer = new unsigned char[dataLength+1];
        dataBuffer[dataLength] = 0;
        is.read(reinterpret_cast<char*>(dataBuffer), dataLength);
        is.close();
    }
}

void BufferFile::Destroy()
{
    if(dataBuffer != nullptr)
    {
        delete[] dataBuffer;
        dataBuffer = nullptr;
        dataLength = 0;
    }
}

BufferFile::~BufferFile()
{
    Destroy();
}

BufferFile::BufferFile(BufferFile&& bufferFile) noexcept
{
    this->dataBuffer = bufferFile.dataBuffer;
    this->dataLength = bufferFile.dataLength;
    bufferFile.dataBuffer = nullptr;
    bufferFile.dataLength = 0;
}

BufferFile& BufferFile::operator=(BufferFile&& bufferFile) noexcept
{
    this->dataBuffer = bufferFile.dataBuffer;
    this->dataLength = bufferFile.dataLength;
    bufferFile.dataBuffer = nullptr;
    bufferFile.dataLength = 0;
    return *this;
}

bool FileExists(const std::string_view filename)
{
#ifdef __APPLE__
	const fs::path p = std::string(filename);
#else
	const fs::path p = filename;
#endif
	return fs::exists(p);
}

bool IsRegularFile(const std::string_view filename)
{
#ifdef __APPLE__
	const fs::path p = std::string(filename);
#else
	const fs::path p = filename;
#endif
	return fs::is_regular_file(p);

}

bool IsDirectory(const std::string_view filename)
{
#ifdef __APPLE__
	const fs::path p = std::string(filename);
#else
	const fs::path p = filename;
#endif
	return fs::is_directory(p);
}

void IterateDirectory(const std::string_view dirname, std::function<void(const std::string_view)> func, bool recursive)
{

	if (IsDirectory(dirname))
	{
#ifdef __APPLE__
		for (auto& p : fs::directory_iterator(std::string(dirname)))
#else
		for (auto& p : fs::directory_iterator(dirname))
#endif
		{
			if (IsRegularFile(p.path().generic_string()))
			{
				func(p.path().generic_string());
			}
			else if (recursive && IsDirectory(p.path().generic_string()))
			{
				IterateDirectory(p.path().generic_string(), func, recursive);
			}
		}
	}
	else
	{
		logDebug(fmt::format("[Error] Path: {}  is not a directory!", dirname));
	}
}

size_t CalculateFileSize(const std::string& filename)
{
	std::ifstream in(filename, std::ifstream::binary | std::ifstream::ate);
	return static_cast<size_t>(in.tellg());
}

bool CreateDirectory(const std::string_view dirname)
{
#ifdef __APPLE__
	return fs::create_directory(std::string(dirname));
#else
	return fs::create_directory(dirname);
#endif
}

bool RemoveDirectory(const std::string_view dirname, bool removeAll)
{
	if (removeAll)
	{
#ifdef  __APPLE__
		return fs::remove_all(std::string(dirname));
#else
		return fs::remove_all(dirname);
#endif
	}
	else
	{
#ifdef __APPLE__
		return fs::remove(std::string(dirname));
#else
		return fs::remove(dirname);
#endif
	}
	}


std::string GetFilenameExtension(const std::string_view path)
{
	const fs::path p = path;
	return p.extension().string();
}

std::string GetFileParentPath(const std::string_view path)
{
#ifdef __APPLE__
	fs::path p = std::string(path);
#else
	fs::path p = fs::path(path);
#endif
	return p.parent_path().string();
}

std::string LinkFolderAndFile(const std::string_view folderPath, const std::string_view filePath)
{

#ifdef __APPLE__
	fs::path f = std::string(folderPath);
	fs::path p = std::string(filePath);
#else
	const fs::path f = fs::path(folderPath);
	const fs::path p = fs::path(filePath);
#endif
	const fs::path l = f / p;

	return MakeGeneric(l.string());
}

void WriteStringToFile(const std::string& path, const std::string_view content)
{
	std::ofstream t(path);
	t << content;

}

std::string GetFilename(const std::string_view path)
{
	const fs::path p = path;
	return p.filename().string();
}

std::string GetStem(const std::string_view path)
{
	const fs::path p = path;
	return p.stem().string();
}

std::string MakeGeneric(const std::string_view path)
{
	std::string p = path.data();
	std::replace_if(p.begin(), p.end(),
		[](char separator) {return separator == '\\'; }, '/');
	return p;
}

std::string GetRelativePath(const std::string_view path, const std::string_view relative)
{
	fs::path p = fs::path(path);
	//logDebug(std::string("Relative path from: ") + path.data() + " to: " + relative.data());
	return MakeGeneric(fs::relative(p, relative).string());
}

std::string GetCurrentPath()
{
	return fs::current_path().string();
}

const std::string LoadFile(const std::string& path)
{
	std::ifstream t(path);
	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	return str;
}


}
#endif

namespace neko
{


}
