#pragma once

#include <assimp/IOSystem.hpp>
#include <utilities/file_utility.h>

#include <assimp/IOStream.hpp>

namespace neko
{

class NekoIOStream : public Assimp::IOStream
{
	
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
