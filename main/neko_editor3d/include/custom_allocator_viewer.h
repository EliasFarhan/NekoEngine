#pragma once

#include <engine/system.h>
#include "engine/custom_allocator.h"

namespace neko::editor
{

class CustomAllocatorViewer : public DrawImGuiInterface
{
public:
	void DrawImGui() override;
private:
	Allocator* allocator_ = nullptr;
};

}
