//
// Created by efarhan on 11/22/20.
//
#include "engine/filesystem.h"

#ifdef STD_FILESYSTEM

#include <filesystem>

namespace fs = std::filesystem;
#endif

#ifdef EASY_PROFILE_USE
#include <easy/profiler.h>
#endif

namespace neko
{

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

void BufferFile::Destroy()
{
    if (dataBuffer != nullptr)
    {
        delete[] dataBuffer;
        dataBuffer = nullptr;
        dataLength = 0;
    }
}
LoadingAssetJob::LoadingAssetJob(const FilesystemInterface & filesystem) : Job(
        [this] {
#ifdef EASY_PROFILE_USE
            EASY_BLOCK("Load File in AssetJob");
#endif
            bufferFile_.Destroy();
            bufferFile_ = filesystem_.LoadFile(filePath_);
        }),
                                                                     filesystem_(filesystem)
{
}

void LoadingAssetJob::SetFilePath(std::string_view path)
{
    filePath_ = path;
}

void LoadingAssetJob::Reset()
{
    Job::Reset();
    bufferFile_.Destroy();
}




BufferFile Filesystem::LoadFile(std::string_view path) const
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Load File from Filesystem");
#endif
    BufferFile newFile;
    if (FileExists(path))
    {
        std::ifstream is(path.data(), std::ifstream::binary);
        if (!is)
        {
            logDebug(fmt::format("[Error] Could not open file: {}  for BufferFile", path));
        }
        else
        {
            is.seekg(0, is.end);
            newFile.dataLength = is.tellg();
            is.seekg(0, is.beg);
            newFile.dataBuffer = static_cast<unsigned char*>(malloc(newFile.dataLength + 1));
            newFile.dataBuffer[newFile.dataLength] = 0;
            is.read(reinterpret_cast<char*>(newFile.dataBuffer), newFile.dataLength);
            is.close();
        }
    }
    return newFile;
}

bool Filesystem::FileExists(std::string_view filename) const
{
    const fs::path p = filename;
    return fs::exists(p);
}

bool Filesystem::IsRegularFile(std::string_view filename) const
{
    const fs::path p = filename;
    return fs::is_regular_file(p);
}

bool Filesystem::IsDirectory(std::string_view filename) const
{
    const fs::path p = filename;
    return fs::is_directory(p);
}
}