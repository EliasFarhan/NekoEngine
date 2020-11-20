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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include "argh.h"


#include "json.hpp"
#include <iomanip>
using json = nlohmann::json;

int main(int argc, char** argv)
{
    argh::parser cmdl(argv);
    if(!cmdl(1))
    {
        return EXIT_FAILURE;
    }
    std::string filename;
    cmdl(1) >> filename;
    int width;
    int height;
    int channelCount;
    stbi_info(filename.c_str(), &width, &height, &channelCount);
    json imageInfo;
    imageInfo["width"] = width;
    imageInfo["height"] = height;
    imageInfo["channelCount"] = channelCount;

    std::cout << std::setw(4) << imageInfo << std::endl;
}