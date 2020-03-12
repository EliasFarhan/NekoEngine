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
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLEutilities/ FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <functional>
#include <string_view>

namespace neko::physfs
{
	/**
     * \brief To always call before ANY PhysFS operations
     * \return True if successful, false if not
     */
    bool InitPhysFs();

    /**
     * \brief Checks if file exists in one of the search paths.
     * \param filename the filename to check
     * \return True if found, false if not
     */
    bool FileExists(std::string_view filename);

    /**
     * \brief Opens an archive, for its files to be read, and adds it to the search path.
     * \param path The path to the archive
     * \return True if it succeeds, false if it does not.
     */
    bool OpenArchive(const std::string& path);

	const char* ReadFile(const std::string& path);
}