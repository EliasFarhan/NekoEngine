#pragma once

#include <SFML/Graphics/RenderTexture.hpp>

namespace editor
{

class PrefabViewer
{
public:
    void Update(float dt);
private:
    sf::RenderTexture previewTexture_;
};

}