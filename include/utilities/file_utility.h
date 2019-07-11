#pragma once

/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

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

namespace neko
{

bool FileExists(const std::string_view filename);

bool IsRegularFile(const std::string_view filename);

bool IsDirectory(const std::string_view filename);

void IterateDirectory(const std::string_view dirname, std::function<void(const std::string_view)> func);

std::ifstream::pos_type CalculateFileSize(const std::string& filename);

bool CreateDirectory(const std::string_view dirname);

bool RemoveDirectory(const std::string_view dirname, bool removeAll = true);

const std::string LoadFile(const std::string& path);

std::string LinkFolderAndFile(const std::string_view folderPath, const std::string_view filePath);

std::string GetFilenameExtension(const std::string_view path);

std::string GetFileParentPath(const std::string_view path);
}
