#pragma once

#include <engine/entity.h>


namespace editor
{
class NekoEditor;
class Inspector
{
public:
    explicit Inspector(NekoEditor& nekoEditor) : nekoEditor_(nekoEditor){ }
    void ShowEntityInfo(neko::Entity entity);

private:
    NekoEditor& nekoEditor_;
};
}