#pragma once

#include <engine/resource.h>
#include <xxhash.hpp>
#include <utilities/service_locator.h>

namespace neko
{

using TextureId = std::uint32_t;
const TextureId INVALID_TEXTURE_ID = 0;
using TextureIndex = std::uint32_t;
const TextureIndex INVALID_TEXTURE_INDEX = std::numeric_limits<TextureIndex>::max();
using TexturePathHash = xxh::hash32_t;
struct Image
{
    unsigned char* data = nullptr;
    int width = -1, height = -1;
    int nbChannels = 0;
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
	/**
	 * \brief This function is called if the file was loaded
	 */
    bool IsLoaded() const;

    void FreeImage();

    TextureId GetTextureId() const { return textureId_;};
protected:
    void Reset();
    virtual void CreateTexture() = 0;
    Job uploadToGpuJob_;
    Job convertImage_;
    ResourceJob diskLoadJob_;
    Image image_;
    TextureId textureId_ = 0;
};

class TextureManagerInterface
{
public:
    virtual TextureIndex LoadTexture(std::string_view path) = 0;
    virtual TextureId GetTextureId(TextureIndex index) = 0;
};

class NullTextureManager : public TextureManagerInterface
{
public:
    TextureIndex LoadTexture([[maybe_unused]] std::string_view path) override
    {
	    return INVALID_TEXTURE_INDEX;
    }
    TextureId GetTextureId([[maybe_unused]] TextureIndex index) override
    {
	    return INVALID_TEXTURE_ID;
    }
};
	
class TextureManager : public SystemInterface, public TextureManagerInterface
{
public:
    TextureManager();
	void Init() override {}
    void Destroy() override;
protected:
    TextureIndex currentIndex_ = 0;
    std::unordered_map<std::string, TextureIndex> textureIndexMap_;
};

using TextureManagerLocator = Locator<TextureManagerInterface, NullTextureManager>;

}