#pragma once

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

#include <assimp/IOSystem.hpp>
#include <utils/file_utility.h>

#include <assimp/IOStream.hpp>

namespace neko
{
/**
 * \brief Read-only IO Stream use by Assimp that use Neko BufferFile
 */
class NekoIOStream : public Assimp::IOStream
{
public:
    explicit NekoIOStream(const FilesystemInterface&);
    size_t Read(void* pvBuffer, size_t pSize, size_t pCount) override;

    size_t Write(const void* pvBuffer, size_t pSize, size_t pCount) override;

    aiReturn Seek(size_t pOffset, aiOrigin pOrigin) override;

    size_t Tell() const override;

    size_t FileSize() const override;

    void Flush() override;

    void LoadFromFile(std::string_view path);
    void Destroy();
private:
    const FilesystemInterface& filesystem_;
    BufferFile bufferFile_;
    size_t cursorIndex_ = 0;
};


class NekoIOSystem : public Assimp::IOSystem
{
public:
    explicit NekoIOSystem(const FilesystemInterface&);
	bool Exists(const char* pFile) const override
	{
		return filesystem_.FileExists(pFile);
	}

	char getOsSeparator() const override {
#ifdef WIN32
		return '\\';
#else
		return '/';
#endif
	};
	Assimp::IOStream* Open(const char* pFile, const char* pMode) override;
	void Close(Assimp::IOStream* pFile) override;

protected:
    const FilesystemInterface& filesystem_;
};
}
