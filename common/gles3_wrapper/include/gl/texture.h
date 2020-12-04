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
#include <string_view>
#include <graphics/texture.h>
#include "gl/gles3_include.h"

struct ktxTexture;
namespace neko::gl
{
class TextureLoader
{
public:
    enum class TextureLoaderError : std::uint8_t
    {
        NONE = 0u,
        ASSET_LOADING_ERROR = 1u,
        DECOMPRESS_ERROR = 2u,
        UPLOAD_TO_GPU_ERROR = 3u
    };
    explicit TextureLoader(std::string_view path,
                           TextureId,
                           Texture::TextureFlags flags = Texture::DEFAULT);
    TextureLoader(const TextureLoader&) = delete;
    TextureLoader& operator=(const TextureLoader&) = delete;
    std::string_view GetPath() const;
    TextureLoader(TextureLoader&&) noexcept ;
    TextureLoader& operator=(TextureLoader&&) = default;
    void Start();
    [[nodiscard]] bool IsDone();
    [[nodiscard]] TextureId GetTextureId() const;
    [[nodiscard]] const Texture& GetTexture() const;
    [[nodiscard]] bool HasErrors() const;
    [[nodiscard]] TextureLoaderError GetErrors() const;
private:
    void LoadTexture();
    void DecompressTexture();
    void UploadToGL();

    std::reference_wrapper<const FilesystemInterface> filesystem_;
    TextureId textureId_;
    std::string path_;
    Texture::TextureFlags flags_ = Texture::DEFAULT;
    ktxTexture* kTexture = nullptr;
    Job loadingTextureJob_;
    Job decompressTextureJob_;
    Job uploadToGLJob_;
    BufferFile bufferFile_;
    Texture texture_;
    TextureLoaderError error_ = TextureLoaderError::NONE;
};

class TextureManager : public neko::TextureManagerInterface, public SystemInterface
{
public:
    explicit TextureManager();
    TextureId LoadTexture(std::string_view path, Texture::TextureFlags flags) override;

    const Texture* GetTexture(TextureId index) const override;
    [[nodiscard]] TextureName GetTextureName(TextureId textureId) const;
    bool IsTextureLoaded(TextureId textureId) const override;

    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

private:
    const FilesystemInterface& filesystem_;
    std::map<std::string, TextureId> texturePathMap_;
    std::map<TextureId, Texture> textureMap_;
    std::queue<TextureLoader> textureLoaders_;
};

TextureName StbCreateTexture(const std::string_view filename, const FilesystemInterface& filesystem,
                             Texture::TextureFlags flags = Texture::DEFAULT);
TextureName CreateTextureFromKTX(const std::string_view filename, const FilesystemInterface& filesystem);
TextureName LoadCubemap(std::vector<std::string> facesFilename, const FilesystemInterface& filesystem);
void DestroyTexture(TextureName);



}