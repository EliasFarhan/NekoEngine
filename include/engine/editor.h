#pragma once

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include "system.h"

namespace neko
{
class GraphicsManager;
class Editor : public System
{
public:
    void Init() override;

    void Update() override;

    void Destroy() override;

    void AddInspectorInfo(const std::string_view name, const std::string_view value);

    GraphicsManager* graphicsManager;
    sf::RenderWindow* renderWindow = nullptr;
private:
    std::vector<std::pair<std::string, std::string>> inspectorValues[2];
};
}