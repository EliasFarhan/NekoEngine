#include "gl/texture.h"
#include "gl/gles3_include.h"
#include "utilities/file_utility.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <sstream>
#include <engine/log.h>

namespace neko::gl
{

unsigned stbCreateTexture(const char* filename, bool smooth, bool mipMaps, bool clampWrap)
{
    std::string extension = GetFilenameExtension(filename);
    if(!FileExists(filename))
    {
        std::ostringstream oss;
        oss << "[Error] Texture: " << filename << " does not exist\n";
        logDebug(oss.str());
        return 0;
    }
    int width, height, nrChannels;

    int reqComponents = 0;
    if (extension == ".jpg" || extension == ".tga" || extension == ".hdr")
        reqComponents = 3;
    else if (extension == ".png")
        reqComponents = 4;

    void *data = nullptr;
    if(extension == ".hdr")
    {
        data = stbi_loadf(filename, &width, &height, &reqComponents, 0);
    }
    else
    {
        data = stbi_load(filename, &width, &height, &nrChannels, reqComponents);
    }
    if (data == nullptr)
    {
        std::ostringstream oss;
        oss << "[Error] Texture: cannot load " << filename << "\n";
        logDebug(oss.str());
        return 0;
    }
    unsigned int texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampWrap ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampWrap ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smooth ? GL_LINEAR : GL_NEAREST);
    if(mipMaps)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smooth ? GL_LINEAR : GL_NEAREST);
    }
    if (extension == ".jpg" || extension == ".tga")
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else if (extension == ".png")
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else if(extension == ".hdr")
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
    }
    if (mipMaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);
    return texture;
}
}