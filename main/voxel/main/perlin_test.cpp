#include <array>

#include "PerlinNoise.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char** argv)
{
    const size_t textureSize = 1024;
    const int octave = 1;
    const float scale = 10.0f;
    siv::PerlinNoise perlinNoise;
    std::vector<std::array<float, textureSize>> perlinValues(textureSize);
    for(size_t i = 0; i < textureSize; i++)
    {
        for(size_t j = 0; j < textureSize; j++)
        {
            const float x = float(i) / textureSize * scale;
            const float y = float(j) / textureSize * scale;
            perlinValues[i][j] = float(perlinNoise.octaveNoise(x, y, octave)) / 2.0f + 0.5f;
        }
    }
    stbi_write_hdr("img.hdr", textureSize, textureSize, 1, &perlinValues[0][0]);
    return EXIT_SUCCESS;
}