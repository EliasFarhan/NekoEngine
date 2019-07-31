#include <tools/inspector.h>
#include <tools/neko_editor.h>
#include <imgui.h>
namespace editor
{

void Inspector::ShowEntityInfo(neko::Entity entity)
{
    if(entity == neko::INVALID_ENTITY)
        return;

    auto& entityManager = nekoEditor_.GetEntityManager();

    if(entityManager.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::POSITION2D)))
    {
        auto& positionManager = nekoEditor_.GetPositionManager();
        auto& pos = positionManager.GetComponent(entity);
        float posRaw[2] = {pos.x, pos.y};
        if(ImGui::InputFloat2("Position", posRaw))
        {
            positionManager.SetComponent(entity, sf::Vector2f(posRaw[0], posRaw[1]));
        }
    }
}

void Inspector::BeginWindow()
{
    ImGui::Begin("Inspector");
}

void Inspector::EndWindow()
{
    ImGui::End();
}
}