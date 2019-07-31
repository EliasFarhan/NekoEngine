#pragma once

#include <SFML/Graphics/RenderTexture.hpp>

namespace editor
{
class SceneViewer
{
public:
    void Update(sf::RenderTexture& sceneTexture);
};
}