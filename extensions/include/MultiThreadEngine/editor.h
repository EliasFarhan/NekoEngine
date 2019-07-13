#pragma once

#include <engine/editor.h>
#inclued <gra

namespace multi
{
/**
 * \brief used in the graphics manager to show informations from the engine/game/systems in used
 */
class MultiEditor : public neko::Editor
{
public:
    void Init() override;

    void Update(float dt) override;

    void Destroy() override;

    void AddInspectorInfo(const std::string_view name, const std::string_view value) override;

protected:
    std::vector<std::pair<std::string, std::string>> inspectorValues_[2];
};
}