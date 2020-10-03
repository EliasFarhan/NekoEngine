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
#include <mathematics/vector.h>

namespace neko
{

using Color3 = Vec3f;
using Color4 = Vec4f;

namespace Color
{
    const static Color3 black = Color3(0.0f,0.0f,0.0f);
    const static Color3 blue = Color3(0.0f,0.0f,1.0f);
    const static Color4 transparent = Color4(0.0f,0.0f,0.0f,0.0f);
    const static Color3 cyan = Color3(0.0f,1.0f,1.0f);
    const static Color3 gray = Color3(0.5f, 0.5f,0.5f);
    const static Color3 green = Color3(0,1,0);
    const static Color3 grey = gray;
    const static Color3 magenta = Color3(1,0,1);
    const static Color3 red = Color3(1,0,0);
    const static Color3 white = Color3(1,1,1);
    const static Color3 yellow = Color3(1,0.92f, 0.016f);
}
}
