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

#include "io_system.h"

namespace neko
{

Assimp::IOStream* NekoIOSystem::Open(const char* pFile, [[maybe_unused]] const char* pMode)
{
    const std::string mode = pMode;
    if(mode == "r+" or mode == "w" or mode == "w+" or mode == "a" or mode == "a+")
    {
        return nullptr;
    }
    auto* ioStream = new NekoIOStream(filesystem_);
    ioStream->LoadFromFile(pFile);
	return ioStream;
}

void NekoIOSystem::Close(Assimp::IOStream* pFile)
{
    auto* ioStream = static_cast<NekoIOStream*>(pFile);
    ioStream->Destroy();
}

NekoIOSystem::NekoIOSystem(const FilesystemInterface& filesystem):
    filesystem_(filesystem)
{

}

size_t NekoIOStream::Read(void* pvBuffer, size_t pSize, size_t pCount)
{
    if(!pvBuffer)
        return 0;
    if (cursorIndex_ + pCount * pSize > bufferFile_.dataLength)
        return 0;
    std::memcpy(pvBuffer, bufferFile_.dataBuffer+cursorIndex_,pSize*pCount);
    cursorIndex_ += pSize*pCount;
    return pSize*pCount;
}

size_t NekoIOStream::Write([[maybe_unused]]const void* pvBuffer, [[maybe_unused]]size_t pSize, [[maybe_unused]]size_t pCount)
{
    return 0;
}

aiReturn NekoIOStream::Seek(size_t pOffset, aiOrigin pOrigin)
{
    switch(pOrigin)
    {
        case aiOrigin_CUR:
            cursorIndex_ += pOffset;
            return aiReturn_SUCCESS;
            break;
        case aiOrigin_SET:
            cursorIndex_ = pOffset;
            return aiReturn_SUCCESS;
            break;
        case aiOrigin_END:
            cursorIndex_ = bufferFile_.dataLength-pOffset;
            return aiReturn_SUCCESS;
            break;
        default:
            break;
    }
    return aiReturn_FAILURE;
}

size_t NekoIOStream::Tell() const
{
    return cursorIndex_;
}

size_t NekoIOStream::FileSize() const
{
    return bufferFile_.dataLength;
}

void NekoIOStream::Flush()
{

}

void NekoIOStream::LoadFromFile(std::string_view path)
{
    bufferFile_ = filesystem_.LoadFile(path);
    cursorIndex_ = 0;
}

void NekoIOStream::Destroy()
{
    bufferFile_.Destroy();
}

NekoIOStream::NekoIOStream(const FilesystemInterface& filesystem) :
    filesystem_(filesystem)
{

}
}

