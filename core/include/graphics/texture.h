#pragma once

#include <engine/resource.h>

namespace neko
{

using TextureId = std::uint32_t;
struct Image
{
    unsigned char* data;
    int width, height;
    int nbChannels;
    void Destroy();
};

Image StbImageConvert(BufferFile imageFile);

/**
 * \brief Neko Texture contains an Image loaded async from disk, converted by stb_image
 */
class Texture
{
public:
    Texture();
    virtual void Destroy() = 0;
    void SetPath(std::string_view path);
    /**
     * \brief This function schedules the resource load from disk and the image conversion
     */
    void LoadFromDisk();
    bool IsLoaded();
    /**
     * \brief This function needs to be called on the Render Thread
     */
    virtual void LoadTextureToGpu() = 0;

    void FreeImage();

    TextureId GetTextureId(){return textureId_;};
protected:
    Job convertImage_;
    ResourceJob diskLoadJob_;
    Image image_;
    TextureId textureId_ = 0;
};
template <class T>
class TextureManager : public ResourceManager<T>
{

};
}