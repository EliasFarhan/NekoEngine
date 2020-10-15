#include "gl/font.h"

#include "engine/engine.h"

namespace neko::gl
{

void FontManager::Init()
{
  const auto& config = BasicEngine::GetInstance()->config;
  textShader_.LoadFromFile(config.dataRootPath + "shaders/engine/text.vert",
    config.dataRootPath + "shaders/engine/text.frag");
  // All functions return a value different than 0 whenever an error occurred
 
}

void FontManager::LoadFont(std::string_view fontName, int pixelHeight)
{
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
  {
    logDebug("[Error] Freetype could not init FreeType Library");
    return;
  }
  FT_Face face;
  BufferFile fontFile;
  fontFile.Load(fontName);
  if (FT_New_Memory_Face(ft, 
    fontFile.dataBuffer, 
    fontFile.dataLength,
    0, 
    &face))
  {
    logDebug("[Error] Freetype: Failed to load font");
    return;
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
    characters[c] = character;
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  // destroy FreeType once we're finished
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

void FontManager::RenderText(std::string_view text, Vec2f position, float scale,
    Color4 color)
{
}

void FontManager::Destroy()
{
}

void FontManager::Render()
{
}
}
