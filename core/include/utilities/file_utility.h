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

#include <functional>

#include <string>
#include <string_view>
#include <fstream>

#include "engine/jobsystem.h"


#if defined(__ANDROID__)
#include <jni.h>
extern "C"
{
JNIEXPORT void JNICALL
Java_swiss_sae_gpr5300_MainActivity_load(JNIEnv *env, [[maybe_unused]] jclass clazz, jobject mgr);
}
#endif

namespace neko
{
/**
 * \brief Non RAII structure, please Destroy it
 */
struct BufferFile
{
    BufferFile() = default;
    ~BufferFile();
    BufferFile(BufferFile&& bufferFile) noexcept;
    BufferFile& operator=(BufferFile&& bufferFile) noexcept;
    BufferFile(const BufferFile&) = delete;
    BufferFile& operator= (const BufferFile&) = delete;

    char* dataBuffer = nullptr;
    size_t dataLength = 0;

    void Load(std::string_view path);
    void Destroy();

};
class ResourceJob : public Job
{
public:
    ResourceJob();
    void SetFilePath(std::string_view path);
    std::string GetFilePath() const {return filePath_; }
    const BufferFile& GetBufferFile() const {return bufferFile_;}
    void Reset() override;
private:
    std::string filePath_;
    BufferFile bufferFile_;
};

bool FileExists(const std::string_view filename);

bool IsRegularFile(const std::string_view filename);

bool IsDirectory(const std::string_view filename);

void IterateDirectory(const std::string_view dirname, std::function<void(const std::string_view)> func, bool recursive=false);

size_t CalculateFileSize(const std::string& filename);

std::string GetCurrentPath();

bool CreateDirectory(const std::string_view dirname);

bool RemoveDirectory(const std::string_view dirname, bool removeAll = true);

const std::string LoadFile(const std::string& path);


std::string LinkFolderAndFile(const std::string_view folderPath, const std::string_view filePath);
std::string GetRelativePath(const std::string_view path, const std::string_view relative);
std::string GetFilenameExtension(const std::string_view path);

std::string GetFileParentPath(const std::string_view path);
std::string GetFilename(const std::string_view path);
std::string GetStem(const std::string_view path);

std::string MakeGeneric(const std::string_view path);

void WriteStringToFile(const std::string& path, const std::string_view content);
}
