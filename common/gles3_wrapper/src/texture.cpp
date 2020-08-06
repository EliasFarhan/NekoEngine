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

#include "gl/texture.h"
#include "gl/gles3_include.h"
#include "gli/gli.hpp"

#include "utilities/file_utility.h"

#include <sstream>
#include <engine/log.h>
#include <graphics/texture.h>
#include <engine/engine.h>


#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif
namespace neko::gl
{
TextureId gliCreateTexture(const std::string_view filename, Texture::TextureFlags flags)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Create GLI Texture");
#endif
    gli::texture texture = gli::load(filename.data());
    if (texture.empty())
        return 0;

    gli::gl GL(gli::gl::PROFILE_ES30);
    const gli::gl::format  format = GL.translate(texture.format(), texture.swizzles());
    GLenum target = GL.translate(texture.target());
    assert(gli::is_compressed(texture.format()) && target == gli::TARGET_2D);

    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(target, textureId);
    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(texture.levels() - 1));
    glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, format.Swizzles[0]);
    glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, format.Swizzles[1]);
    glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, format.Swizzles[2]);
    glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, format.Swizzles[3]);
    const Vec3<GLsizei>  extent(texture.extent());
    glTexStorage2D(target, static_cast<GLint>(texture.levels()), format.Internal, extent.x, extent.y);
    for (std::size_t level = 0; level < texture.levels(); ++level)
    {
        const Vec3<GLsizei> extent(texture.extent(level));
        glCompressedTexSubImage2D(
            target, static_cast<GLint>(level), 0, 0, extent.x, extent.y,
            format.Internal, static_cast<GLsizei>(texture.size(level)), texture.data(0, 0, level));
    }

    return textureId;
}

TextureId stbCreateTexture(const std::string_view filename, Texture::TextureFlags flags)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Create Texture");
    EASY_BLOCK("Load From File");
#endif
    const std::string extension = GetFilenameExtension(filename);
    if (!FileExists(filename))
    {
        std::ostringstream oss;
        oss << "[Error] Texture: " << filename << " does not exist\n";
        logDebug(oss.str());
        return 0;
    }

    int reqComponents = 0;
    if (extension == ".jpg" || extension == ".tga" || extension == ".hdr")
        reqComponents = 3;
    else if (extension == ".png")
        reqComponents = 4;
    BufferFile textureFile;
    textureFile.Load(filename);
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    Image image = StbImageConvert(textureFile);
    /*if (extension == ".hdr")
    {
        //data = stbi_loadf(filename.data(), &width, &height, &reqComponents, 0);
        data = stbi_loadf_from_memory((unsigned char*) (textureFile.dataBuffer),
                                      textureFile.dataLength, &width, &height, &reqComponents, 0);
    }*/

    textureFile.Destroy();
    if (image.data == nullptr)
    {
        std::ostringstream oss;
        oss << "[Error] Texture: cannot load " << filename << "\n";
        logDebug(oss.str());
        return INVALID_TEXTURE_ID;
    }
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Push Texture To GPU");
#endif
    TextureId texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, flags & Texture::CLAMP_WRAP ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, flags & Texture::CLAMP_WRAP ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, flags & Texture::SMOOTH_TEXTURE ? GL_LINEAR : GL_NEAREST);
    if (flags & Texture::MIPMAPS_TEXTURE)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        flags & Texture::SMOOTH_TEXTURE ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, flags & Texture::SMOOTH_TEXTURE ? GL_LINEAR : GL_NEAREST);
    }
    if (extension == ".jpg" || extension == ".tga")
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
    }
    else if (extension == ".png")
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
    }
    else if (extension == ".hdr")
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, image.width, image.height, 0, GL_RGB, GL_FLOAT, image.data);
    }
    if (flags & Texture::MIPMAPS_TEXTURE)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    image.Destroy();
    return texture;
}

TextureId LoadCubemap(std::vector<std::string> facesFilename)
{
    TextureId textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);


    for (unsigned int i = 0; i < facesFilename.size(); i++)
    {
        BufferFile textureFile;
        textureFile.Load(facesFilename[i]);
        const auto extension = GetFilenameExtension(facesFilename[i]);
        int reqComponents = 0;
        if (extension == ".jpg" || extension == ".tga" || extension == ".hdr")
            reqComponents = 3;
        else if (extension == ".png")
            reqComponents = 4;
        Image image = StbImageConvert(textureFile);
        textureFile.Destroy();
        if (image.data != nullptr)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data
            );
        }
        else
        {
            logDebug("[Error] Cubemap tex failed to load at path: "+ facesFilename[i]);
        }
        image.Destroy();
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glCheckError();
    return textureID;
}

void DestroyTexture(TextureId textureId)
{
    glDeleteTextures(1, &textureId);
    textureId = INVALID_TEXTURE_ID;

}

void Texture::CreateTexture()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Generate Texture");
#endif
    glCheckError();
    glGenTextures(1, &textureId_);
    
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    glBindTexture(GL_TEXTURE_2D, textureId_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, flags_ & CLAMP_WRAP ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, flags_ & CLAMP_WRAP ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, flags_ & SMOOTH_TEXTURE ? GL_LINEAR : GL_NEAREST);
    glCheckError();
	if (flags_ & MIPMAPS_TEXTURE)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        flags_ & SMOOTH_TEXTURE ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);
        glCheckError();
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, flags_ & SMOOTH_TEXTURE ? GL_LINEAR : GL_NEAREST);
        glCheckError();
    }
    
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Copy Buffer");
#endif
    GLenum internalFormat = 0;
    GLenum dataFormat = 0;
	if(flags_ & HDR)
	{
		switch(image_.nbChannels)
		{
        case 1:
            logDebug("Loading texture: " + diskLoadJob_.GetFilePath() + " as GL_R16F");
            internalFormat = GL_R16F;
            dataFormat = GL_RED;
            break;
        case 2:

            logDebug("Loading texture: " + diskLoadJob_.GetFilePath() + " as GL_RG16F");
            internalFormat = GL_RG16F;
            dataFormat = GL_RG;
            break;
        case 3:

            logDebug("Loading texture: " + diskLoadJob_.GetFilePath() + " as GL_RGB16F");
            internalFormat = GL_RGB16F;
            dataFormat = GL_RGB;
            break;
        case 4:

            logDebug("Loading texture: " + diskLoadJob_.GetFilePath() + " as GL_RGBA16F");
            internalFormat = GL_RGBA16F;
            dataFormat = GL_RGBA;
            break;
		default:
            break;
		}

	}
    else
    {
        switch (image_.nbChannels)
        {
        case 1:
        {
            logDebug("Loading texture: " + diskLoadJob_.GetFilePath() + " as GL_R8");
            internalFormat = GL_R8;
            dataFormat = GL_RED;
            break;
        }
        case 2:
        {
            internalFormat = GL_RG8;
            dataFormat = GL_RG;
            break;
        }
        case 3:
        {

            logDebug("Loading texture: " + diskLoadJob_.GetFilePath() + " as GL_RGB8");
            internalFormat = flags_ & GAMMA_CORRECTION ? GL_SRGB8 : GL_RGB8;
            dataFormat = GL_RGB;
            break;
        }
        case 4:
        {

            logDebug("Loading texture: " + diskLoadJob_.GetFilePath() + " as GL_RGBA8");
            internalFormat = flags_ & GAMMA_CORRECTION ? GL_SRGB8_ALPHA8 : GL_RGBA8;
            dataFormat = GL_RGBA;
            break;
        }
        default:
            break;
        }
    }
	if(!(flags_ & HDR))
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image_.width, image_.height, 0, dataFormat, GL_UNSIGNED_BYTE, image_.data);
		glCheckError();
	}
    else
    {
	    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image_.width, image_.height, 0, dataFormat, GL_FLOAT, (float*)image_.data);
	    glCheckError();
    }

#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif

    if (flags_ & MIPMAPS_TEXTURE)
    {
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("Generate Mipmaps");
#endif
        glGenerateMipmap(GL_TEXTURE_2D);
        glCheckError();
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    
}

void Texture::Destroy()
{
    DestroyTexture(textureId_);
    textureId_ = INVALID_TEXTURE_ID;
    Reset();
}


TextureManager::TextureManager()
{
}

TextureIndex TextureManager::LoadTexture(std::string_view path)
{
    const auto it = textureIndexMap_.find(path.data());
    if (it != textureIndexMap_.end())
    {
        return it->second;
    }

    if (!FileExists(path))
    {
        return INVALID_TEXTURE_INDEX;
    }
    const auto textureIndex = static_cast<TextureIndex>(textures_.size());
	textures_.emplace_back(std::make_unique<Texture>());
    auto* texture = textures_.back().get();
    texture->SetPath(path);
    texture->LoadFromDisk();
    return textureIndex;
	
}

void TextureManager::Update(seconds dt)
{
	
}

void TextureManager::Destroy()
{
	for(auto& texture : textures_)
	{
        texture->Destroy();
	}
    neko::TextureManager::Destroy();
    textures_.clear();
}

TextureId TextureManager::GetTextureId(TextureIndex index)
{
    if (index == INVALID_TEXTURE_INDEX)
        return INVALID_TEXTURE_ID;
    auto* texture = textures_[index].get();
    return texture->GetTextureId();
}
}



