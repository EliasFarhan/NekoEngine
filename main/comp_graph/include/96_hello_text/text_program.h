#pragma once

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
	struct Character {
		unsigned int textureID; // ID handle of the glyph texture
		Vec2i   size;      // Size of glyph
		Vec2i   bearing;   // Offset from baseline to left/top of glyph
		unsigned int advance;   // Horizontal offset to advance to next glyph
	};
	void RenderText(gl::Shader& shader, std::string_view text, float x, float y, float scale, Color3 color);
	gl::Shader textShader_;
	Mat4f projection_;
	std::array<Character, 128> characters_;
	gl::VertexArrayObject textureQuad_;
};
}
