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


Image StbImageConvert(BufferFile imageFile, bool flipY, bool hdr)
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
Texture::Texture() :
	uploadToGpuJob_([this]
	{
#ifdef EASY_PROFILE_USE
            EASY_BLOCK("Create GPU Texture");
#endif
		CreateTexture();
	}),
    convertImageJob_([this]
    {
        const auto filename = diskLoadJob_.GetFilePath();
        const auto extension = GetFilenameExtension(filename);
       
    	image_ = StbImageConvert(diskLoadJob_.GetBufferFile(), flags_ & FLIP_Y,flags_ & HDR);
	    diskLoadJob_.GetBufferFile().Destroy();
	    RendererLocator::get().AddPreRenderJob(&uploadToGpuJob_);
    })
{

}

void Texture::LoadFromDisk()
{
    if (textureId_ == INVALID_TEXTURE_ID)
    {
        BasicEngine::GetInstance()->ScheduleJob(&diskLoadJob_, JobThreadType::RESOURCE_THREAD);
        convertImageJob_.AddDependency(&diskLoadJob_);
        BasicEngine::GetInstance()->ScheduleJob(&convertImageJob_, JobThreadType::OTHER_THREAD);
    }
}

bool Texture::IsLoaded() const
{
    return uploadToGpuJob_.IsDone();
}


void Texture::SetPath(std::string_view path)
{
    diskLoadJob_.SetFilePath(path);
}

void Texture::FreeImage()
{
    image_.Destroy();
}

void Texture::Reset()
{
    diskLoadJob_.Reset();
    convertImageJob_.Reset();
    uploadToGpuJob_.Reset();
}

TextureManager::TextureManager()
{
}

void TextureManager::Destroy()
{
    textureIndexMap_.clear();
    currentIndex_ = 0;
}

void Image::Destroy()
{
    stbi_image_free(data);
    data = nullptr;
    height = -1;
    width = -1;
}
}
