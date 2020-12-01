#pragma once

#include <string_view>
#include "engine/jobsystem.h"

#if defined(WIN32) || defined(__linux__)
#define STD_FILESYSTEM
#endif

namespace neko
{

/**
 * \brief RAII structure, please Destroy it
 */
struct BufferFile
{
    BufferFile() = default;
    ~BufferFile();
    BufferFile(BufferFile&& bufferFile) noexcept;
    BufferFile& operator=(BufferFile&& bufferFile) noexcept;
    BufferFile(const BufferFile&) = delete;
    BufferFile& operator= (const BufferFile&) = delete;

    unsigned char* dataBuffer = nullptr;
    size_t dataLength = 0;
    void Destroy();

};

/**
 * \brief Simple interface to a filesystem, useful to define
 * specialized filesystem like physfs, android or consoles
 */
class FilesystemInterface
{
public:
    virtual ~FilesystemInterface() = default;
    [[nodiscard]] virtual BufferFile LoadFile(std::string_view path) const = 0;
    [[nodiscard]] virtual bool FileExists(std::string_view) const = 0;
    [[nodiscard]] virtual bool IsRegularFile(std::string_view) const = 0;
    [[nodiscard]] virtual bool IsDirectory(std::string_view) const = 0;
};

class LoadingAssetJob : public Job
{
public:
    explicit LoadingAssetJob(const FilesystemInterface&);
    void SetFilePath(std::string_view path);
    std::string GetFilePath() const {return filePath_; }
    const BufferFile& GetBufferFile() const {return bufferFile_;}
    void Reset() override;
private:
    const FilesystemInterface& filesystem_;
    std::string filePath_;
    BufferFile bufferFile_;
};



class Filesystem : public FilesystemInterface
{
public:
    BufferFile LoadFile(std::string_view path) const override;

    bool FileExists(std::string_view view) const override;

    bool IsRegularFile(std::string_view view) const override;

    bool IsDirectory(std::string_view view) const override;

};
}