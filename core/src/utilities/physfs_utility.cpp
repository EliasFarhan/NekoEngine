#include <utilities/physfs_utility.h>
#include <engine/log.h>

#include <physfs.h>

namespace neko::physfs
{
bool InitPhysFs()
{
	return PHYSFS_init(PHYSFS_getBaseDir());
}

bool FileExists(std::string_view filename)
{
	if (PHYSFS_exists(filename.data())) 
	{
		LogDebug(LogCategory::IO, "Found '" + std::string(filename) + "'.");
		return true;
	}
	
	LogError(LogCategory::IO, "Could not find '" + std::string(filename) + "'!");
	return false;

}

bool OpenArchive(const std::string& path)
{
	LogDebug(LogCategory::IO, "Mounting '" + path + "'...");
	if (PHYSFS_mount(path.data(), nullptr, 1))
	{
		LogDebug(LogCategory::IO, "Successfully mounted '" + path + "'");
		return true;
	}

	LogError(LogCategory::IO, "Could not open archive '" + path + "'");
	return false;
}

bool SetWriteDir(const std::string& path)
{
	if (PHYSFS_setWriteDir(path.data()))
	{
		LogDebug(LogCategory::IO, "WriteDir set to '" + path + "'");
		return true;
	}

	LogError(LogCategory::IO, "Could not set writeDir to '" + path + "'");
	return false;
}

std::string ReadFile(const std::string& path)
{
	PHYSFS_File* file = PHYSFS_openRead(path.data());

	if (file == nullptr) {
		LogError(LogCategory::IO, "Could not open file '" + path + "'");
		return "";
	}

	const PHYSFS_sint64 len = PHYSFS_fileLength(file);
	PHYSFS_seek(file, 0);
	auto* buffer = new char[len + 1];
	PHYSFS_readBytes(file, buffer, len);
	buffer[len] = 0;
	std::string output = buffer;
	delete[] buffer;
	return output;
}

int WriteStringToFile(const std::string& path, std::string_view content)
{
	PHYSFS_File* file = PHYSFS_openWrite(path.data());

	if (file == nullptr) {
		LogError(LogCategory::IO, "Could not open file '" + path + "'");
		return 0;
	}

	return PHYSFS_writeBytes(file, content.data(), content.length());
}

const char* GetLastError()
{
	return PHYSFS_getLastError();
}
}
