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
	void TextureManager::CreateTexture()
	{
        const auto textureId = currentUploadedTexture_.textureId;
        const auto flags = currentUploadedTexture_.flags;
        auto& image = currentUploadedTexture_.image;
        if (image.data == nullptr)
        {
            textureMap_[textureId] = {};
            return;
        }
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("Generate Texture");
#endif
        TextureName texture;
        glCheckError();
        glGenTextures(1, &texture);

#ifdef EASY_PROFILE_USE
        EASY_END_BLOCK;
#endif
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, flags& Texture::CLAMP_WRAP ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, flags& Texture::CLAMP_WRAP ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, flags& Texture::SMOOTH_TEXTURE ? GL_LINEAR : GL_NEAREST);
        glCheckError();
        if (flags & Texture::MIPMAPS_TEXTURE)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                flags & Texture::SMOOTH_TEXTURE ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);
            glCheckError();
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, flags & Texture::SMOOTH_TEXTURE ? GL_LINEAR : GL_NEAREST);
            glCheckError();
        }

#ifdef EASY_PROFILE_USE
        EASY_BLOCK("Copy Buffer");
#endif
        GLenum internalFormat = 0;
        GLenum dataFormat = 0;
        if (flags & Texture::HDR)
        {
            switch (image.nbChannels)
            {
            case 1:
                internalFormat = GL_R16F;
                dataFormat = GL_RED;
                break;
            case 2:

                internalFormat = GL_RG16F;
                dataFormat = GL_RG;
                break;
            case 3:

                internalFormat = GL_RGB16F;
                dataFormat = GL_RGB;
                break;
            case 4:

                internalFormat = GL_RGBA16F;
                dataFormat = GL_RGBA;
                break;
            default:
                break;
            }

        }
        else
        {
            switch (image.nbChannels)
            {
            case 1:
            {
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

                internalFormat = flags & Texture::GAMMA_CORRECTION ? GL_SRGB8 : GL_RGB8;
                dataFormat = GL_RGB;
                break;
            }
            case 4:
            {

                internalFormat = flags & Texture::GAMMA_CORRECTION ? GL_SRGB8_ALPHA8 : GL_RGBA8;
                dataFormat = GL_RGBA;
                break;
            }
            default:
                break;
            }
        }
        if (!(flags & Texture::HDR))
        {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.width, image.height, 0, dataFormat, GL_UNSIGNED_BYTE, image.data);
            glCheckError();
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.width, image.height, 0, dataFormat, GL_FLOAT, (float*)image.data);
            glCheckError();
        }

#ifdef EASY_PROFILE_USE
        EASY_END_BLOCK;
#endif

        if (flags & Texture::MIPMAPS_TEXTURE)
        {
#ifdef EASY_PROFILE_USE
            EASY_BLOCK("Generate Mipmaps");
#endif
            glGenerateMipmap(GL_TEXTURE_2D);
            glCheckError();
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        textureMap_[textureId] = {texture, {currentUploadedTexture_.image.width, currentUploadedTexture_.image.height}};

	}

	void TextureManager::Destroy()
	{
		for(auto& textureName : textureMap_)
		{
            DestroyTexture(textureName.second.name);
            textureName.second.name = INVALID_TEXTURE_NAME;
		}
        neko::TextureManager::Destroy();
	}


TextureName stbCreateTexture(const std::string_view filename, Texture::TextureFlags flags)
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
        return INVALID_TEXTURE_NAME;
    }
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Push Texture To GPU");
#endif
    TextureName texture;
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

TextureName LoadCubemap(std::vector<std::string> facesFilename)
{
    TextureName textureID;
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

void DestroyTexture(TextureName textureName)
{
    glDeleteTextures(1, &textureName);
    textureName = INVALID_TEXTURE_NAME;

}
}



