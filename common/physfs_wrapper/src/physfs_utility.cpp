#include <physfs_utility.h>
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

const char* ReadFile(const std::string& path)
{
	PHYSFS_File* file = PHYSFS_openRead(path.data());

	if (file == nullptr) {
		LogError(LogCategory::IO, "Could not open file '" + path + "'");
		return "";
	}

	const PHYSFS_sint64 len = PHYSFS_fileLength(file);
	PHYSFS_seek(file, 0);
	void* buffer = new void*[len + 1];
	PHYSFS_readBytes(file, &buffer, len);
	const char* result = static_cast<const char*>(buffer);
	return result;
}
}
