#pragma once
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
#include <queue>
#include <engine/log.h>
#include <engine/resource.h>
#include <xxhash.hpp>
#include <sole.hpp>
#include <utilities/service_locator.h>

namespace neko
{
/**
 * \brief Using OpenGL naming convention. A texture name is returned by glGenTextures
 */
using TextureName = std::uint32_t;
const TextureName INVALID_TEXTURE_NAME = 0;
/**
 * \brief Unique identifier (16 bytes UUID for the texture in the texture manager.
 * Generated at compile time and loaded at init.
 */
using TextureId = sole::uuid;
const TextureId INVALID_TEXTURE_ID = sole::uuid();
using TexturePathHash = xxh::hash32_t;
struct Image
{
    Image() = default;
    ~Image();
    Image(Image&& image) noexcept;

    Image& operator=(Image&& image) noexcept;
    Image(const Image&) = delete;
    Image& operator= (const Image&) = delete;


    unsigned char* data = nullptr;
    int width = -1, height = -1;
    int nbChannels = 0;
    void Destroy();
};

Image StbImageConvert(BufferFile imageFile, bool flipY=false, bool hdr = false);

/**
 * \brief Neko Texture contains an Image loaded async from disk, converted by stb_image
 */
namespace Texture
{
enum TextureFlags : unsigned
{
    SMOOTH_TEXTURE = 1u << 0u,
    MIPMAPS_TEXTURE = 1u << 1u,
    CLAMP_WRAP = 1u << 2u,
    REPEAT_WRAP = 1u << 3u,
    MIRROR_REPEAT_WRAP = 1u << 4u,
    GAMMA_CORRECTION = 1u << 5u,
    FLIP_Y = 1u << 6u,
    HDR = 1u << 7u,
    DEFAULT = REPEAT_WRAP | SMOOTH_TEXTURE | MIPMAPS_TEXTURE,

};
}

class TextureManagerInterface
{
public:
	virtual ~TextureManagerInterface() = default;
	virtual TextureId LoadTexture(std::string_view path, Texture::TextureFlags flags = Texture::DEFAULT) = 0;
    virtual TextureName GetTextureId(TextureId index) const = 0;
    [[nodiscard]] virtual bool IsTextureLoaded(TextureId textureId) const = 0;
};

class NullTextureManager : public TextureManagerInterface
{
public:
    TextureId LoadTexture([[maybe_unused]] std::string_view path, Texture::TextureFlags flags = Texture::DEFAULT) override
    {
        logDebug("[Warning] Using NullTextureManager to Load Texture");
	    return INVALID_TEXTURE_ID;
    }
    TextureName GetTextureId([[maybe_unused]] TextureId index) const override
    {
        logDebug("[Warning] Using NullTextureManager to Get Texture Id");
	    return INVALID_TEXTURE_NAME;
    }
    bool IsTextureLoaded(TextureId textureId) const override  { return false; }
};

class TextureManager;
	
class TextureLoader
{
public:
    explicit TextureLoader(TextureManager& textureManager);

    void SetTextureFlags(Texture::TextureFlags textureFlags) { flags_ = textureFlags; }
    void SetTextureId(TextureId textureId);
	/**
	 * \brief This function schedules the resource load from disk and the image conversion.
	 * Must be called after setting the texture id 
	 */
    void LoadFromDisk();

    [[nodiscard]] bool IsLoaded() const { return diskLoadJob_.IsDone(); }
    [[nodiscard]] bool HasStarted() const { return convertImageJob_.HasStarted(); }
    void Reset();
private:
    TextureManager& textureManager_;
    Texture::TextureFlags flags_ = Texture::DEFAULT;
    Job convertImageJob_;
    ResourceJob diskLoadJob_;
    Image image_;
    TextureId textureId_ = INVALID_TEXTURE_ID;
};

struct TextureInfo
{
	TextureInfo() = default;
	~TextureInfo() = default;
    TextureInfo(TextureInfo&& image) noexcept = default;
    TextureInfo& operator=(TextureInfo&& image) noexcept = default;
    TextureInfo(const TextureInfo&) = delete;
    TextureInfo& operator= (const TextureInfo&) = delete;
	
    TextureId textureId = INVALID_TEXTURE_ID;
    Image image;
    Texture::TextureFlags flags = Texture::DEFAULT;
};

class TextureManager : public TextureManagerInterface
{
public:
    TextureManager();
    TextureId LoadTexture(std::string_view path, Texture::TextureFlags flags = Texture::DEFAULT) override;
    std::string GetPath(TextureId textureId) const;
    void Update();
	
    virtual void Destroy();
    virtual void UploadToGpu(TextureInfo&& texture);

	TextureName GetTextureId(TextureId index) const override;
	bool IsTextureLoaded(TextureId textureId) const override;
protected:
	/**
	 * \brief Called on the renderer pre render
	 */
    virtual void CreateTexture() = 0;
    std::unordered_map<TextureId, std::string> texturePathMap_;
    std::unordered_map<TextureId, TextureName> textureNameMap_;
    std::queue<TextureInfo> texturesToLoad_;
    std::queue<TextureInfo> texturesToUpload_;
    TextureLoader textureLoader_;
    TextureInfo currentUploadedTexture_;
    Job uploadToGpuJob_;
};
using TextureManagerLocator = Locator<TextureManagerInterface, NullTextureManager>;

}