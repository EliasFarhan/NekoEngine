#include <tools/scene_viewer.h>
#include <imgui.h>
#include <imgui-SFML.h>
namespace editor
{

void SceneViewer::Update(sf::RenderTexture& sceneTexture)
{
    if (ImGui::BeginTabItem("Scene Viewer"))
    {
        auto tintColor = sf::Color::White;
        auto borderColor = sf::Color::White;
        const auto imWindowSize = ImGui::GetWindowSize();
        auto windowSize = sf::Vector2f(imWindowSize.x, imWindowSize.y);
        const auto ratio = sf::Vector2f(windowSize.x / sceneTexture.getSize().x,
                                        windowSize.y / sceneTexture.getSize().y);
        windowSize = sf::Vector2f(sceneTexture.getSize()) * (ratio.x < ratio.y ? ratio.x : ratio.y);
        ImGui::Image(sceneTexture.getTexture(), sf::Vector2f(windowSize.x - 25.f, windowSize.y - 40.f), tintColor,
                     borderColor);
        ImGui::EndTabItem();
    }
}
}