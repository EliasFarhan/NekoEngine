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

#include "utils/file_utility.h"

#include <sstream>
#include <engine/log.h>
#include <graphics/texture.h>
#include <engine/engine.h>

#include <fmt/core.h>


#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif
#include <utils/json_utility.h>
namespace neko::gl
{

void TextureManager::Destroy()
{
    for (auto& textureName : textureMap_)
    {
        DestroyTexture(textureName.second.name);
        textureName.second.name = INVALID_TEXTURE_NAME;
    }
}


TextureName
StbCreateTexture(const std::string_view filename, const FilesystemInterface& filesystem, Texture::TextureFlags flags)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Create Texture");
    EASY_BLOCK("Load From File");
#endif
    const std::string extension = GetFilenameExtension(filename);
    if (!filesystem.FileExists(filename))
    {
        logError(fmt::format("Texture: {} does not exist", filename));
        return 0;
    }

    int reqComponents = 0;
    if (extension == ".jpg" || extension == ".tga" || extension == ".hdr")
        reqComponents = 3;
    else if (extension == ".png")
        reqComponents = 4;
    BufferFile textureFile = filesystem.LoadFile(filename);
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
        logError(fmt::format("Texture: cannot load {}", filename));
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


TextureName CreateTextureFromKTX(const std::string_view filename, const FilesystemInterface& filesystem)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Load KTX Texture");
    EASY_BLOCK("Open File");
#endif
    BufferFile textureFile = filesystem.LoadFile(filename);
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif

#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Create KTX from memory");
#endif
    gli::gl glProfile(gli::gl::PROFILE_ES30);

    auto texture = gli::load(reinterpret_cast<const char*>(textureFile.dataBuffer), textureFile.dataLength);
    if (texture.empty())
    {
        logError("Could not load texture with GLI");
        return INVALID_TEXTURE_NAME;
    }
    const gli::gl::format format = glProfile.translate(texture.format(), texture.swizzles());

    GLenum target = glProfile.translate(texture.target());
    logDebug(fmt::format("texture format: {}, texture target {}, is compressed {}",
                         texture.format(),
                         texture.target(),
                         is_compressed(texture.format())));
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif

#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Upload Texture to GPU");
#endif
    TextureName textureName = 0;
    glGenTextures(1, &textureName);
    glBindTexture(target, textureName);

    glCheckError();
    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(texture.levels() - 1));

    glCheckError();
    glm::tvec3<GLsizei> extent{ texture.extent() };
    glTexStorage2D(target, static_cast<GLint>(texture.levels()), format.Internal, extent.x, extent.y);

    glCheckError();
    for (std::size_t level = 0; level < texture.levels(); ++level)
    {
        glm::tvec3<GLsizei> levelExtent(texture.extent(level));
        if(gli::is_compressed(texture.format()))
        {
            glCompressedTexSubImage2D(
                    target, static_cast<GLint>(level), 0, 0, levelExtent.x, levelExtent.y,
                    format.Internal, static_cast<GLsizei>(texture.size(level)), texture.data(0, 0, level));
        }
        else
        {
            glTexSubImage2D(
                    target, static_cast<GLint>(level), 0, 0, levelExtent.x, levelExtent.y,
                    format.Internal, static_cast<GLsizei>(texture.size(level)), texture.data(0, 0, level));
        }
        glCheckError();
    }
    glCheckError();
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    return textureName;
}

TextureName LoadCubemap(std::vector<std::string> facesFilename, const FilesystemInterface& filesystem)
{
    TextureName textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);


    for (unsigned int i = 0; i < facesFilename.size(); i++)
    {
        BufferFile textureFile = filesystem.LoadFile(facesFilename[i]);
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
            logError(fmt::format("Cubemap tex failed to load at path: {}", facesFilename[i]));
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

TextureId TextureManager::LoadTexture(std::string_view path, Texture::TextureFlags flags)
{
    const auto it = texturePathMap_.find(path.data());
    if (it != texturePathMap_.end())
    {
        return it->second;
    }
    const std::string metaPath = fmt::format("{}.meta",path);
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Load JSON");
#endif
    auto metaJson = LoadJson(metaPath);
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    TextureId textureId = INVALID_TEXTURE_ID;
    std::string ktxPath;
    if(CheckJsonExists(metaJson, "uuid"))
    {
        textureId = sole::rebuild(metaJson["uuid"].get<std::string>());
    }
    else
    {
        logError(fmt::format("Could not find texture id in json file {}", metaPath));
        return textureId;
    }
    if(CheckJsonExists(metaJson, "ktx_path"))
    {
        ktxPath = metaJson["ktx_path"];
    }
    else
    {
        logError("Could not find ktx path in json file");
        return INVALID_TEXTURE_ID;
    }

    if (textureId == INVALID_TEXTURE_ID)
    {
        logError(" Invalid texture id on texture load");
        return textureId;
    }
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    textureLoaders_.push(TextureLoader
    {
        config.dataRootPath + ktxPath,
        textureId,
        flags
    });
    textureLoaders_.back().Start();
    texturePathMap_[path.data()] = textureId;
    return textureId;
}

const Texture* TextureManager::GetTexture(TextureId index) const
{
    const auto it = textureMap_.find(index);
    if(it != textureMap_.end())
    {
        return &it->second;
    }
    return nullptr;
}

bool TextureManager::IsTextureLoaded(TextureId textureId) const
{
    const auto it = textureMap_.find(textureId);
    return it != textureMap_.end();
}

void TextureManager::Init()
{
    TextureManagerLocator::provide(this);
}

void TextureManager::Update([[maybe_unused]]seconds dt)
{
    while(!textureLoaders_.empty())
    {
        auto& textureLoader = textureLoaders_.front();
        if(textureLoader.HasErrors())
        {
            switch (textureLoader.GetErrors())
            {
            case TextureLoader::TextureLoaderError::ASSET_LOADING_ERROR:
                logError(fmt::format(" Could not load texture {} from disk", textureLoader.GetPath()));
                break;
            case TextureLoader::TextureLoaderError::DECOMPRESS_ERROR:
                logError(fmt::format(" Could not decompress texture {} from disk", textureLoader.GetPath()));
                break;
            case TextureLoader::TextureLoaderError::UPLOAD_TO_GPU_ERROR:
                logError(fmt::format(" Could not upload texture {} from disk", textureLoader.GetPath()));
                break;
            default:
                break;
            }
            textureLoaders_.pop();
        }
        else if(textureLoader.IsDone())
        {
            textureMap_[textureLoader.GetTextureId()] = textureLoader.GetTexture();
            textureLoaders_.pop();
        }
        else
        {
            break;
        }
    }
}

TextureManager::TextureManager() :
    filesystem_(BasicEngine::GetInstance()->GetFilesystem())
{
    
}

TextureName TextureManager::GetTextureName(TextureId textureId) const
{
    const auto* texture = GetTexture(textureId);
    if(texture == nullptr)
    {
        return INVALID_TEXTURE_NAME;
    }
    return texture->name;
}

TextureLoader::TextureLoader(std::string_view path,
                             TextureId textureId,
                             Texture::TextureFlags flags) :
        filesystem_(BasicEngine::GetInstance()->GetFilesystem()),
        textureId_(textureId), path_(path), flags_(flags),
        loadingTextureJob_([this]() { LoadTexture(); }),
        decompressTextureJob_([this]() { DecompressTexture(); }),
        uploadToGLJob_([this](){ UploadToGL(); })
{

}

bool TextureLoader::IsDone()
{
    return uploadToGLJob_.IsDone();
}

void TextureLoader::LoadTexture()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Load KTX from disk");
#endif
    bufferFile_ = filesystem_.get().LoadFile(path_);
    if(bufferFile_.dataBuffer == nullptr)
    {
        error_ = TextureLoaderError::ASSET_LOADING_ERROR;
        return;
    }
    BasicEngine::GetInstance()->ScheduleJob(&decompressTextureJob_, JobThreadType::OTHER_THREAD);
}

void TextureLoader::DecompressTexture()
{
    {
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("Create KTX from memory");
#endif
        texture_.gliTexture = gli::load(reinterpret_cast<const char*>(
                bufferFile_.dataBuffer),
                                        bufferFile_.dataLength);
    }
    if (texture_.gliTexture.empty())
    {
        logError(" OpenGLI error while opening KTX content");
        error_ = TextureLoaderError::DECOMPRESS_ERROR;
        return;
    }
    RendererLocator::get().AddPreRenderJob(&uploadToGLJob_);
}

void TextureLoader::UploadToGL()
{
#ifdef EASY_PROFILE_USE
      EASY_BLOCK("Upload KTX Texture to GPU");
#endif
    gli::gl glProfile(gli::gl::PROFILE_ES30);

    auto& texture = texture_.gliTexture;
    const auto isCompressed = gli::is_compressed(texture.format());
    const auto format = glProfile.translate(texture.format(), texture.swizzles());
    const auto target = glProfile.translate(texture.target());
    glGenTextures(1, &texture_.name);
    glBindTexture(target, texture_.name);

    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(texture.levels() - 1));

    glm::tvec3<GLsizei> extent{ texture.extent() };
    glTexStorage2D(target, static_cast<GLint>(texture.levels()), format.Internal, extent.x, extent.y);
    for (std::size_t level = 0; level < texture.levels(); ++level)
    {
        glm::tvec3<GLsizei> levelExtent(texture.extent(level));
        if(isCompressed)
        {
            glCompressedTexSubImage2D(
                    target, static_cast<GLint>(level), 0, 0, levelExtent.x, levelExtent.y,
                    format.Internal, static_cast<GLsizei>(texture.size(level)), texture.data(0, 0, level));
        }
        else
        {
            glTexSubImage2D(
                    target, static_cast<GLint>(level), 0, 0, levelExtent.x, levelExtent.y,
                    format.Internal, static_cast<GLsizei>(texture.size(level)), texture.data(0, 0, level));
        }
    }
    glCheckError();

}

void TextureLoader::Start()
{
    BasicEngine::GetInstance()->ScheduleJob(&loadingTextureJob_, JobThreadType::RESOURCE_THREAD);
}

std::string_view TextureLoader::GetPath() const
{
    return path_;
}

TextureLoader::TextureLoader(TextureLoader&& textureLoader) noexcept :
    filesystem_(BasicEngine::GetInstance()->GetFilesystem()),
    textureId_(textureLoader.textureId_),
    path_(textureLoader.path_),
    flags_(textureLoader.flags_),
    loadingTextureJob_([this]() { LoadTexture(); }),
    decompressTextureJob_([this]() { DecompressTexture(); }),
    uploadToGLJob_([this](){ UploadToGL(); })
{
}

TextureId TextureLoader::GetTextureId() const
{
    return textureId_;
}

const Texture& TextureLoader::GetTexture() const
{
    return texture_;
}

bool TextureLoader::HasErrors() const
{
    return error_ != TextureLoaderError::NONE;
}

TextureLoader::TextureLoaderError TextureLoader::GetErrors() const
{
    return error_;
}
}




