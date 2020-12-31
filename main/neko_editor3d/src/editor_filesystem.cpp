
#include <editor_filesystem.h>

neko::BufferFile neko::editor::EditorFilesystem::LoadFile(std::string_view path) const
{
    return filesystem_.LoadFile(path);
}

bool neko::editor::EditorFilesystem::FileExists(std::string_view view) const
{
    return filesystem_.FileExists(view);
}

bool neko::editor::EditorFilesystem::IsRegularFile(std::string_view view) const
{
    return filesystem_.IsRegularFile(view);
}

bool neko::editor::EditorFilesystem::IsDirectory(std::string_view view) const
{
    return filesystem_.IsDirectory(view);
}
