//
// Created by efarhan on 11/22/20.
//
#include "engine/filesystem.h"

#include <fstream>
#include "engine/log.h"
#include <fmt/format.h>

#ifdef STD_FILESYSTEM

#include <filesystem>

namespace fs = std::filesystem;
#endif

#ifdef TRACY_ENABLE
#include <Tracy.hpp>
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
LoadingAssetTask::LoadingAssetTask(const FilesystemInterface & filesystem) :
    Task(
        [this] {
#ifdef TRACY_ENABLE
            ZoneNamedN(loadFile, "Load File in AssetJob", true);
#endif
            bufferFile_.Destroy();
            bufferFile_ = filesystem_.LoadFile(filePath_);
        }),
    filesystem_(filesystem)
{
}

void LoadingAssetTask::SetFilePath(std::string_view path)
{
    filePath_ = path;
}

void LoadingAssetTask::Reset()
{
    Task::Reset();
    bufferFile_.Destroy();
}




BufferFile Filesystem::LoadFile(std::string_view filename) const
{
#ifdef TRACY_ENABLE
    ZoneScoped;
#endif
    BufferFile newFile;
    if (FileExists(filename))
    {
        std::ifstream is(filename.data(), std::ifstream::binary);
        if (!is)
        {
            logError(fmt::format("Could not open file: {} for BufferFile", filename));
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