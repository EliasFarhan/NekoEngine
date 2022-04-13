#pragma once
/*
MIT License

Copyright (c) 2019 SAE Institute Switzerland AG

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

#include "engine/filesystem.h"
#include "engine/system.h"

namespace neko::physfs
{
class PhysFsFilesystem : public FilesystemInterface, public SystemInterface
{
public:
    explicit PhysFsFilesystem(const char* argv0);
    [[nodiscard]] BufferFile LoadFile(std::string_view path) const override;

    [[nodiscard]] bool FileExists(std::string_view filename) const override;

    [[nodiscard]] bool IsRegularFile(std::string_view view) const override;

    [[nodiscard]] bool IsDirectory(std::string_view filename) const override;

    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void AddMount(std::string_view dir, std::string_view mountPoint, int append) const;
protected:
    std::string argv0;

};
}