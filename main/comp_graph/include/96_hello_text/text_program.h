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
#include <ft2build.h>
#include FT_FREETYPE_H


#include "comp_graph/sample_program.h"
#include "gl/shader.h"
#include "gl/shape.h"
#include "gl/texture.h"

namespace neko
{
class HelloTextProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	struct Character
    {
		TextureName textureID = 0; // ID handle of the glyph texture
		Vec2i   size;      // Size of glyph
		Vec2i   bearing;   // Offset from baseline to left/top of glyph
		long advance = 0;   // Horizontal offset to advance to next glyph
	};
	void RenderText(gl::Shader& shader, std::string_view text, float x, float y, float scale, Color3 color);
	gl::Shader textShader_;
	Mat4f projection_;
	std::array<Character, 128> characters_;
	gl::VertexArrayObject textureQuad_;
};
}
