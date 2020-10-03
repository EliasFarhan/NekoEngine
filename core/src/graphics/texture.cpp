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
#include "graphics/graphics.h"
#include "graphics/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "engine/engine.h"
#include "utilities/file_utility.h"

#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif

namespace neko
{


Image StbImageConvert(const BufferFile& imageFile, bool flipY, bool hdr)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Convert Image");
#endif
    Image image;
	
    stbi_set_flip_vertically_on_load(flipY);
    if (hdr)
    {
        image.data = (unsigned char*)stbi_loadf_from_memory((unsigned char*)(imageFile.dataBuffer),
            imageFile.dataLength, &image.width, &image.height, &image.nbChannels, 0);
    }
    else
    {
        image.data = stbi_load_from_memory((unsigned char*)(imageFile.dataBuffer),
            imageFile.dataLength, &image.width, &image.height, &image.nbChannels, 0);
    }
	return image;
}

TextureLoader::TextureLoader(TextureManager& textureManager) :
	textureManager_(textureManager),
	convertImageJob_([this]
    {
	    logDebug("[Texture Manager] Convert buffer file to image");
        image_ = StbImageConvert(diskLoadJob_.GetBufferFile(), flags_ & Texture::FLIP_Y, flags_ & Texture::HDR);
        TextureInfo textureInfo{ textureId_, std::move(image_), flags_ };
        textureManager_.UploadToGpu(std::move(textureInfo));
        logDebug("[Texture Manager] Finish converting buffer file to image");
    })
{
}

void TextureLoader::SetTextureId(TextureId textureId)
{
	textureId_ = textureId;
	const auto path = textureManager_.GetPath(textureId_);
	diskLoadJob_.SetFilePath(path);
}

void TextureLoader::LoadFromDisk()
{
    if (textureId_ != INVALID_TEXTURE_ID)
    {
#ifndef NEKO_SAMETHREAD
        BasicEngine::GetInstance()->ScheduleJob(&diskLoadJob_, JobThreadType::RESOURCE_THREAD);
        convertImageJob_.AddDependency(&diskLoadJob_);
        BasicEngine::GetInstance()->ScheduleJob(&convertImageJob_, JobThreadType::OTHER_THREAD);
#else
        diskLoadJob_.Execute();
        convertImageJob_.Execute();
#endif
    }
}

void TextureLoader::Reset()
{
	convertImageJob_.Reset();
	diskLoadJob_.Reset();
}



TextureManager::TextureManager() : textureLoader_(*this), uploadToGpuJob_([this]()
{
	CreateTexture();
	currentUploadedTexture_.textureId = INVALID_TEXTURE_ID;
})
{

}

TextureId TextureManager::LoadTexture(std::string_view path, Texture::TextureFlags flags)
{
	
	const std::string metaPath = std::string(path) + ".meta";
    auto metaJson = LoadJson(metaPath);
    TextureId textureId = INVALID_TEXTURE_ID;
	if(CheckJsonExists(metaJson, "uuid"))
	{
        textureId = sole::rebuild(metaJson["uuid"].get<std::string>());
	}
    else
    {
        logDebug("[Error] Could not find texture id in json file");
        return textureId;
    }

    if (textureId == INVALID_TEXTURE_ID)
    {
        logDebug("[Error] Invalid texture id on texture load");
        return textureId;
    }
    const auto it = texturePathMap_.find(textureId);
	if(it != texturePathMap_.end())
	{
		//Texture is already in queue or even loaded
        logDebug("[Texture Manager] Texture is already loaded");
        return textureId;
	}
	std::string log = "[Texture Manager] Loading texture path: ";
	log += path;
	logDebug(log);

    texturePathMap_[textureId] = std::string(path.data());
#ifndef NEKO_SAMETHREAD
	//Put texture in queue
    TextureInfo textureInfo;
    textureInfo.textureId = textureId;
    textureInfo.flags = flags;
    texturesToLoad_.push(std::move(textureInfo));
#else
    textureLoader_.Reset();

    textureLoader_.SetTextureId(textureId);
    textureLoader_.SetTextureFlags(flags);
    textureLoader_.LoadFromDisk();

    auto& textureInfo = texturesToUpload_.front();
    currentUploadedTexture_ = std::move(textureInfo);
    texturesToUpload_.pop();
    uploadToGpuJob_.Reset();
    uploadToGpuJob_.Execute();
#endif
    return textureId;
}

std::string TextureManager::GetPath(TextureId textureId) const
{
	const auto pathIt = texturePathMap_.find(textureId);
	if (pathIt != texturePathMap_.end())
	{
		return pathIt->second;
	}
	return "";
}

void TextureManager::Init()
{
    TextureManagerLocator::provide(this);
}

void TextureManager::Update([[maybe_unused]]seconds dt)
{
#ifndef NEKO_SAMETHREAD
    if (!texturesToLoad_.empty())
    {
        if (textureLoader_.IsLoaded() || !textureLoader_.HasStarted())
        {
            logDebug("[Texture Manager] Loading a texture from disk");
            textureLoader_.Reset();
            const auto& textureInfo = texturesToLoad_.front();
            textureLoader_.SetTextureId(textureInfo.textureId);
            textureLoader_.SetTextureFlags(textureInfo.flags);
            textureLoader_.LoadFromDisk();
            texturesToLoad_.pop();
        }
    }
	if(!texturesToUpload_.empty() && currentUploadedTexture_.textureId == INVALID_TEXTURE_ID)
    {

        logDebug("[Texture Manager] Uploading a texture to the GPU");
		auto& textureInfo = texturesToUpload_.front();
        currentUploadedTexture_ = std::move(textureInfo);
	    texturesToUpload_.pop();
        uploadToGpuJob_.Reset();
	    RendererLocator::get().AddPreRenderJob(&uploadToGpuJob_);
	}
#endif
}

void TextureManager::Destroy()
{
    texturePathMap_.clear();
    textureMap_.clear();
}

void TextureManager::UploadToGpu(TextureInfo&& texture)
{
	texturesToUpload_.push(std::move(texture));
}

Texture TextureManager::GetTexture(TextureId index) const
{
    const auto it = textureMap_.find(index);
	if(it != textureMap_.end())
	{
        return it->second;
	}
    return {};
}

bool TextureManager::IsTextureLoaded(TextureId textureId) const
{
    return textureMap_.find(textureId) != textureMap_.end();
}


Image::Image(Image&& image) noexcept
{
	data = image.data;
	image.data = nullptr;
	width = image.width;
	height = image.height;
	nbChannels = image.nbChannels;
}

Image& Image::operator=(Image&& image) noexcept
{
	data = image.data;
	image.data = nullptr;
	width = image.width;
	height = image.height;
	nbChannels = image.nbChannels;
	return *this;
}

Image::~Image()
{
	Destroy();
}

void Image::Destroy()
{
    if (data)
    {
        stbi_image_free(data);
    }
    data = nullptr;
    height = -1;
    width = -1;
}
}
