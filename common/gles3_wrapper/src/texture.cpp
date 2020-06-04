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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, flags & Texture::SMOOTH_TEXTURE ? GL_LINEAR : GL_NEAREST);
    if (flags & Texture::MIPMAPS_TEXTURE)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
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
    glGenTextures(1, &textureId_);
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    glBindTexture(GL_TEXTURE_2D, textureId_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, flags_ & CLAMP_WRAP ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, flags_ & CLAMP_WRAP ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, flags_ & SMOOTH_TEXTURE ? GL_LINEAR : GL_NEAREST);
    if (flags_ & MIPMAPS_TEXTURE)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        flags_ & SMOOTH_TEXTURE ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, flags_ & SMOOTH_TEXTURE ? GL_LINEAR : GL_NEAREST);
    }
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Copy Buffer");
#endif
    switch (image_.nbChannels)
    {
        case 1:
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, image_.width, image_.height, 0, GL_R8, GL_UNSIGNED_BYTE, image_.data);
            break;
        }
        case 2:
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, image_.width, image_.height, 0, GL_RG, GL_UNSIGNED_BYTE, image_.data);
            break;
        }
        case 3:
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_.width, image_.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_.data);
            break;
        }
        case 4:
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_.width, image_.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_.data);
            break;
        }
        default:
            break;
    }
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    //TODO import hdr textures
    /*else if(extension == ".hdr")
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
    }*/
    if (flags_ & MIPMAPS_TEXTURE)
    {
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("Generate Mipmaps");
#endif
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
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



