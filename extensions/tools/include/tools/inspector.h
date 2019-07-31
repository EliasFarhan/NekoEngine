#pragma once

#include <engine/entity.h>


namespace editor
{
class NekoEditor;
class Inspector
{
public:
    explicit Inspector(NekoEditor& nekoEditor) : nekoEditor_(nekoEditor){ }

    void BeginWindow();
    void ShowEntityInfo(neko::Entity entity);
    void EndWindow();
private:
    NekoEditor& nekoEditor_;
};
}