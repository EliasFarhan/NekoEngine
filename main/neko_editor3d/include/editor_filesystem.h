#pragma once
#include "engine/filesystem.h"

namespace neko::editor
{

class EditorFilesystem : public FilesystemInterface
{
public:
    BufferFile LoadFile(std::string_view path) const override;

    bool FileExists(std::string_view view) const override;

    bool IsRegularFile(std::string_view view) const override;

    bool IsDirectory(std::string_view view) const override;

public:
private:
    Filesystem filesystem_;
};

}
