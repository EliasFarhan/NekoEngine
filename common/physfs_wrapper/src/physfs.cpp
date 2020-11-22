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

#include <physfs.h>
#include <fmt/format.h>

#include <engine/log.h>
#include "fs/physfs.h"


namespace neko::physfs
{


PhysFsFilesystem::PhysFsFilesystem(const char* argv0) : argv0(argv0)
{

}

BufferFile PhysFsFilesystem::LoadFile(std::string_view path) const
{
    BufferFile newFile;
    if(!FileExists(path))
    {
        return newFile;
    }
    auto* file = PHYSFS_openRead(path.data());
    newFile.dataLength = PHYSFS_fileLength(file);
    newFile.dataBuffer = static_cast<unsigned char*>(
            malloc(newFile.dataLength + 1));
    newFile.dataBuffer[newFile.dataLength] = 0;
    if(PHYSFS_readBytes(file, newFile.dataBuffer, newFile.dataLength) == -1)
    {
        logDebug(fmt::format("[Error] Physfs could not read file: {}\nLog: {}",
                             path, PHYSFS_getLastErrorCode()));
        PHYSFS_close(file);
        return {};
    }
    PHYSFS_close(file);
    return newFile;
}

bool PhysFsFilesystem::FileExists(std::string_view filename) const
{
    return PHYSFS_exists(filename.data());
}

bool PhysFsFilesystem::IsRegularFile(std::string_view filename) const
{
    PHYSFS_Stat stat;
    if(PHYSFS_stat(filename.data(), &stat))
    {
        logDebug(fmt::format(
                "[Error] PhysFS could not get stat of file: {}\nLog: {}",
                filename, PHYSFS_getLastErrorCode()));
        return false;
    }
    return stat.filetype == PHYSFS_FILETYPE_REGULAR;
}

bool PhysFsFilesystem::IsDirectory(std::string_view filename) const
{
    PHYSFS_Stat stat;
    if(PHYSFS_stat(filename.data(), &stat))
    {
        logDebug(fmt::format(
                "[Error] PhysFS could not get stat of file: {}\nLog: {}",
                filename, PHYSFS_getLastErrorCode()));
        return false;
    }
    return stat.filetype == PHYSFS_FILETYPE_DIRECTORY;
}

void PhysFsFilesystem::Init()
{
    if(!PHYSFS_init(argv0.c_str()))
    {
        logDebug(fmt::format("[Error] PhysFS init failed {}", PHYSFS_getLastErrorCode()));
        return;
    }
}

void PhysFsFilesystem::Update(seconds dt)
{

}

void PhysFsFilesystem::Destroy()
{
    if (!PHYSFS_deinit())
    {
        logDebug(fmt::format("[Error] PhysFS deinit failed: {}", PHYSFS_getLastErrorCode()));
    }
}

void PhysFsFilesystem::AddMount(std::string_view dir, std::string_view mountPoint, int append) const
{
    if(!PHYSFS_mount(dir.data(), mountPoint.data(), append))
    {
        logDebug(fmt::format(
                "[Error] PhysFS could not mount: {} at mount point: {}\n Log: {}",
                             dir, mountPoint, PHYSFS_getLastErrorCode()));
    }
}

}