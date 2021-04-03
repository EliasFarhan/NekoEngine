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
#include <queue>
#include <map>

#include <xxhash.hpp>
#include <sole.hpp>

#include <engine/assert.h>
#include <engine/log.h>
#include <utils/service_locator.h>
#include <mathematics/vector.h>
#include <engine/filesystem.h>

namespace neko
{

/**
 * \brief Image stores the data from a image file.
 * Used for PNG JPG and other basic image format.
 */
struct Image
{
    Image() = default;
    ~Image();
    Image(Image&& image) noexcept;

    Image& operator=(Image&& image) noexcept;
    Image(const Image&) = delete;
    Image& operator= (const Image&) = delete;


    unsigned char* data = nullptr;
    int width = -1, height = -1;
    int nbChannels = 0;
    void Destroy();
};
/**
 * \brief Simple function decompressing an image for disk to an Image
 * @param imageFile
 * @param flipY
 * @param hdr
 * @return
 */
Image StbImageConvert(const BufferFile& imageFile, bool flipY=false, bool hdr = false);

}