#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include "shader.h"
#include "graphics/font.h"

namespace neko::gl
{
class FontManager : public neko::FontManager
{

public:
  void Init() override;
  void LoadFont(std::string_view fontName, int pixelHeight) override;
  void RenderText(std::string_view text, Vec2f position, float scale,
      Color4 color) override;
  void Destroy() override;

  void Render() override;
protected:
  struct Character
  {
    TextureName textureID = 0; // ID handle of the glyph texture
    Vec2i   size;      // Size of glyph
    Vec2i   bearing;   // Offset from baseline to left/top of glyph
    long advance = 0;   // Horizontal offset to advance to next glyph
  };

  gl::Shader textShader_;
};
}
