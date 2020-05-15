//
// Created by efarhan on 07.05.20.
//

#include "graphics/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "engine/engine.h"

namespace neko
{


Image StbImageConvert(BufferFile imageFile)
{

    Image image;
    image.data = stbi_load_from_memory((unsigned char*) (imageFile.dataBuffer),
                                       imageFile.dataLength, &image.width, &image.height, &image.nbChannels, 0);
    return image;


}
Texture::Texture() : convertImage_([this]{image_ = StbImageConvert(diskLoadJob_.GetBufferFile());})
{
    convertImage_.AddDependency(&diskLoadJob_);
}

void Texture::LoadFromDisk()
{
    BasicEngine::GetInstance()->ScheduleJob(&diskLoadJob_, JobThreadType::RESOURCE_THREAD);
    BasicEngine::GetInstance()->ScheduleJob(&convertImage_, JobThreadType::RESOURCE_THREAD);
}



bool Texture::IsLoaded()
{
    return convertImage_.IsDone();
}

void Texture::SetPath(std::string_view path)
{
    diskLoadJob_.SetFilePath(path);
}

void Texture::FreeImage()
{
    stbi_image_free(image_.data);
}

void Image::Destroy()
{
    stbi_image_free(data);
}
}
