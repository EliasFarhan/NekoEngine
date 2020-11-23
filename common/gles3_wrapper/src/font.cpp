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

#include "gl/font.h"
#include "mathematics/transform.h"
#include "engine/engine.h"
#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif
namespace neko::gl
{

FontManager::FontManager(const FilesystemInterface& filesystem) :
        filesystem_(filesystem)
{

}

void FontManager::Init()
{
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    textShader_.LoadFromFile(config.dataRootPath + "shaders/engine/text.vert",
                             config.dataRootPath + "shaders/engine/text.frag");

    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &textureQuad_.VAO);
    glGenBuffers(1, &textureQuad_.VBO[0]);
    glBindVertexArray(textureQuad_.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, textureQuad_.VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

FontId FontManager::LoadFont(std::string_view fontName, int pixelHeight)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Load Font");
#endif
    const std::string metaPath = std::string(fontName) + ".meta";
    auto metaJson = LoadJson(metaPath);
    FontId fontId = INVALID_FONT_ID;
    if (CheckJsonExists(metaJson, "uuid"))
    {
        fontId = sole::rebuild(metaJson["uuid"].get<std::string>());
    }
    else
    {
        logDebug("[Error] Could not find font id in json file");
        return fontId;
    }

    if (fontId == INVALID_FONT_ID)
    {
        logDebug("[Error] Invalid font id on texture load");
        return fontId;
    }
    auto it = fonts_.find(fontId);
    if (it != fonts_.end())
    {
        //Font is already loaded
        return it->first;
    }

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        logDebug("[Error] Freetype could not init FreeType Library");
        return INVALID_FONT_ID;
    }
    FT_Face face;
    BufferFile fontFile = filesystem_.LoadFile(fontName);
    if (FT_New_Memory_Face(ft,
                           fontFile.dataBuffer,
                           fontFile.dataLength,
                           0,
                           &face))
    {
        logDebug("[Error] Freetype: Failed to load font");
        return INVALID_FONT_ID;
    }
    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, pixelHeight);

    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    std::array<Character, 128> characters;
    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            logDebug("[Error] Freetype failed to load Glyph");
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character =
        {
            texture,
            Vec2i(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            Vec2i(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        characters[c] = character;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    fonts_[fontId] = {characters};
    return fontId;
}

void FontManager::RenderText(FontId font, std::string_view text, Vec2f position, TextAnchor anchor, float scale,
                             Color4 color)
{
    commands_.push_back(
            {font, text.data(), position, anchor, scale, color}
            );
}

void FontManager::Destroy()
{
    for(auto& font : fonts_)
    {
        for(auto& character : font.second.characters)
        {
            glDeleteTextures(1, &character.textureID);
        }
    }
    fonts_.clear();
    glDeleteVertexArrays(1, &textureQuad_.VAO);
}

void FontManager::Render()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Render Font Manager");
#endif
    textShader_.Bind();
    textShader_.SetMat4("projection", projection_);
    for(auto& command : commands_)
    {
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("Render Text");
#endif
        auto& font = fonts_[command.font];
        // activate corresponding render state

        textShader_.SetVec4("textColor", command.color);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(textureQuad_.VAO);

        Vec2f textPosition = CalculateTextPosition(command.position, command.anchor);
        float x = textPosition.x;
        float y = textPosition.y;
        // iterate through all characters
        for (const auto* c = command.text.c_str(); *c != 0; c++)
        {
            const Character ch = font.characters[*c];

            const float xpos = x + ch.bearing.x * command.scale;
            const float ypos = y - (ch.size.y - ch.bearing.y) * command.scale;

            const float w = ch.size.x * command.scale;
            const float h = ch.size.y * command.scale;
            // update VBO for each character
            const float vertices[6][4] = {
                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos,     ypos,       0.0f, 1.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },

                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },
                    { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, textureQuad_.VBO[0]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.advance >> 6) * command.scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    commands_.clear();
}

void FontManager::DestroyFont(FontId font)
{
    auto it = fonts_.find(font);
    if(it == fonts_.end())
        return;
    for(auto& character : it->second.characters)
    {
        glDeleteTextures(1, &character.textureID);
    }
    fonts_.erase(font);
}

Vec2f FontManager::CalculateTextPosition(Vec2f position, TextAnchor anchor)
{
    switch (anchor)
    {
        case TextAnchor::TOP_LEFT:
            return position + windowSize_ * Vec2f::up;
        case TextAnchor::TOP:
            return position + windowSize_ * Vec2f(0.5f, 1.0f);
        case TextAnchor::TOP_RIGHT:
            return position + windowSize_;
        case TextAnchor::CENTER_LEFT:
            return position + windowSize_ * (Vec2f::up/2.0f);
        case TextAnchor::CENTER:
            return position + windowSize_ /2.0f;
        case TextAnchor::CENTER_RIGHT:
            return position + windowSize_ * Vec2f(1.0f, 0.5f);
        case TextAnchor::BOTTOM_LEFT:
            return position;
        case TextAnchor::BOTTOM:
            return position + windowSize_ * (Vec2f::right/2.0f);
        case TextAnchor::BOTTOM_RIGHT:
            return position + windowSize_ * Vec2f::right;
    }
}

void FontManager::SetWindowSize(const Vec2f& windowSize)
{
    windowSize_ = windowSize;
    projection_ = Transform3d::Orthographic(0.0f, windowSize.x, 0.0f, windowSize.y);
}


}
