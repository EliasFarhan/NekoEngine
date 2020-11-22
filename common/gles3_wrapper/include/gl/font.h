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

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "gl/shape.h"
#include "gl/shader.h"
#include "graphics/font.h"

namespace neko::gl
{
struct Character
{
    TextureName textureID = 0; // ID handle of the glyph texture
    Vec2i size;      // Size of glyph
    Vec2i bearing;   // Offset from baseline to left/top of glyph
    long advance = 0;   // Horizontal offset to advance to next glyph
};
struct Font
{
    std::array<Character, 128> characters;
};

class FontManager : public neko::FontManager
{
public:
    explicit FontManager(const FilesystemInterface&);
    void Init() override;

    FontId LoadFont(std::string_view fontName, int pixelHeight) override;

    void RenderText(FontId font, std::string_view text, Vec2f position, TextAnchor anchor, float scale,
                    Color4 color) override;

    void Destroy() override;

    void Render() override;

    void DestroyFont(FontId font) override;
    void SetWindowSize(const Vec2f& windowSize);

protected:
    struct FontRenderingCommand
    {
        FontId font;
        std::string text;
        Vec2f position;
        TextAnchor anchor;
        float scale;
        Color4 color;
    };
    Vec2f CalculateTextPosition(Vec2f position, TextAnchor anchor);

    const FilesystemInterface& filesystem_;
    std::vector<FontRenderingCommand> commands_;
    gl::Shader textShader_;
    gl::VertexArrayObject textureQuad_;
    std::map<FontId, Font> fonts_;
    Vec2f windowSize_;
    Mat4f projection_;
};
}
