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
#include "graphics.h"
#include "graphics/color.h"
#include "mathematics/vector.h"

#include <string_view>
namespace neko
{

using FontId = sole::uuid;
const FontId INVALID_FONT_ID = sole::uuid();

enum class TextAnchor
{
    TOP_LEFT,
    TOP,
    TOP_RIGHT,
    CENTER_LEFT,
    CENTER,
    CENTER_RIGHT,
    BOTTOM_LEFT,
    BOTTOM,
    BOTTOM_RIGHT
};

class FontManager : public RenderCommandInterface
{
public:
  virtual ~FontManager() = default;
  virtual void Init() = 0;
  virtual FontId LoadFont(std::string_view fontName, int pixelHeight) = 0;
  virtual void
  RenderText(FontId font, std::string_view text, Vec2f position, TextAnchor anchor, float scale, Color4 color) = 0;
  virtual void DestroyFont(FontId font) = 0;
  virtual void Destroy() = 0;
};
}
