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
    auto* ioStream = new NekoIOStream();
    ioStream->LoadFromFile(pFile);
	return ioStream;
}

void NekoIOSystem::Close(Assimp::IOStream* pFile)
{
    auto* ioStream = static_cast<NekoIOStream*>(pFile);
    ioStream->Destroy();
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
    bufferFile_.Load(path);
    cursorIndex_ = 0;
}

void NekoIOStream::Destroy()
{
    bufferFile_.Destroy();
}
}

