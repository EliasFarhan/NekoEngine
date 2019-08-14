#pragma once

#include <engine/prefab.h>

namespace editor
{
class NekoEditor;

class EditorPrefabManager : public neko::PrefabManager
{
public:
    explicit EditorPrefabManager(NekoEditor& editor);
    const std::string& GetCurrentPrefabPath() const;
    void SetCurrentPrefabPath(const std::string& currentPrefabPath);
    neko::Index GetCurrentPrefabIndex() const;
    void SetCurrentPrefabIndex(neko::Index currentPrefabIndex);
    bool IsCurrentPrefabTmp();
    void SaveCurrentPrefab();
    neko::Index CreatePrefabFromEntity(neko::Entity entity);
    sf::FloatRect CalculatePrefabBound();
protected:
    void SavePrefab(const std::string_view path);
    std::string currentPrefabPath_ = "";
    neko::Index currentPrefabIndex_ = neko::INVALID_INDEX;
    NekoEditor& nekoEditor_;
};

}