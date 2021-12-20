#pragma once

#include "engine/system.h"

namespace neko::editor
{

class EditorSystem : public SystemInterface, public DrawImGuiInterface
{
protected:
    bool isActive_ = true;
public:
    [[nodiscard]] bool IsActive() const;

    void SetActive(bool isActive);
};

}