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

#include "96_hello_text/text_program.h"
#include "mathematics/transform.h"

namespace neko
{
void HelloTextProgram::Init()
{
	const auto& config = BasicEngine::GetInstance()->GetConfig();
	textShader_.LoadFromFile(config.dataRootPath + "shaders/96_hello_text/text.vert",
		config.dataRootPath + "shaders/96_hello_text/text.frag");
	


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	projection_ = Transform3d::Orthographic(0.0f, config.windowSize.x, 0.0f, config.windowSize.y);
	textShader_.Bind();
	textShader_.SetMat4("projection", projection_);
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		logDebug( "[Error] Freetype could not init FreeType Library");
		return;
	}
	const std::string font_name = config.dataRootPath+"font/8-bit-hud.ttf";
	FT_Face face;
	if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
		logDebug("[Error] Freetype: Failed to load font");
		return;
	}
    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
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
        Character character = {
            texture,
            Vec2i(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            Vec2i(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        characters_[c] = character;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


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

void HelloTextProgram::Update(seconds dt)
{

    std::lock_guard<std::mutex> lock(updateMutex_);
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    projection_ = Transform3d::Orthographic(0.0f, config.windowSize.x, 0.0f, config.windowSize.y);
}

void HelloTextProgram::Destroy()
{
    glDisable(GL_BLEND);
}

void HelloTextProgram::DrawImGui()
{
}

void HelloTextProgram::Render()
{

    std::lock_guard<std::mutex> lock(updateMutex_);
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    glClear(GL_COLOR_BUFFER_BIT);
    textShader_.Bind();
    textShader_.SetMat4("projection", projection_);
    RenderText(textShader_, "Neko Engine!", 25.0f, 25.0f, 1.0f, Color3(0.5, 0.8f, 0.2f));
    RenderText(textShader_, "Meow!", config.windowSize.x/2.0f, config.windowSize.y/2.0f,0.5f, Color3(0.3, 0.7f, 0.9f));
    glEnable(GL_DEPTH_TEST);
}

void HelloTextProgram::OnEvent(const SDL_Event& event)
{
}

void HelloTextProgram::RenderText(gl::Shader& shader, std::string_view text, float x, float y, float scale, Color3 color)
{
    // activate corresponding render state	
    shader.Bind();
    shader.SetVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textureQuad_.VAO);

    // iterate through all characters
    for (const auto* c = text.data(); *c != 0; c++)
    {
	    const Character ch = characters_[*c];

        const float xpos = x + ch.bearing.x * scale;
        const float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        const float w = ch.size.x * scale;
        const float h = ch.size.y * scale;
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
        x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
}
