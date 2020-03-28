#include <custom_allocator_viewer.h>
#include <imgui.h>

namespace neko::editor
{
void CustomAllocatorViewer::DrawImGui()
{
	
	ImGui::Begin("Custom Allocator Profile");
	if(allocator_ != nullptr)
	{
		allocator_->Profile();
	}
	else
	{
		ImGui::TextColored(ImColor(255,0,0,255),"No custom allocator is selected");
	}
	ImGui::End();
}
}
