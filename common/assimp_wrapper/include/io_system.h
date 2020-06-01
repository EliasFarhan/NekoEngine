#pragma once

#include <assimp/IOSystem.hpp>
#include <utilities/file_utility.h>

#include <assimp/IOStream.hpp>

namespace neko
{
/**
 * \brief Read-only IO Stream use by Assimp that use Neko BufferFile
 */
class NekoIOStream : public Assimp::IOStream
{
public:

    size_t Read(void* pvBuffer, size_t pSize, size_t pCount) override;

    size_t Write(const void* pvBuffer, size_t pSize, size_t pCount) override;

    aiReturn Seek(size_t pOffset, aiOrigin pOrigin) override;

    size_t Tell() const override;

    size_t FileSize() const override;

    void Flush() override;

    void LoadFromFile(std::string_view path);
    void Destroy();
private:
    BufferFile bufferFile_;
    size_t cursorIndex_ = 0;
};


class NekoIOSystem : public Assimp::IOSystem
{
public:
	bool Exists(const char* pFile) const override
	{
		return FileExists(pFile);
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
};
}
