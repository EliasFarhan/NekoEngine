#include <utilities/physfs_utility.h>
#include <engine/log.h>

#include <physfs.h>

namespace neko::physfs
{
void InitPhysFs()
{
	PHYSFS_init(nullptr);
}

bool FileExists(const std::string_view filename)
{
	return PHYSFS_exists(filename.data());
}

void WriteStringToFile(const std::string& path, std::string_view content)
{
	PHYSFS_File* file = PHYSFS_openWrite(path.data());

	if (file == nullptr) {
		LogError(LogCategory::IO, "Could not open file '" + path + "'");
		return;
	}

	PHYSFS_writeBytes(file, content.data(), sizeof content);
}
}
