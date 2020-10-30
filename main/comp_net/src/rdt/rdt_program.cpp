#include "rdt/rdt_program.h"
#include "imgui.h"

#include "rdt/rdt_1_0.h"
#include "rdt/rdt_2_0.h"
#include "rdt/rdt_2_1.h"
#include "rdt/rdt_2_2.h"
#include "rdt/rdt_3_0.h"
#include "rdt/go_back_n.h"
#include "rdt/selective_repeat.h"

namespace neko::rdt
{
void HelloRdtProgram::Init()
{
    rdtSamples_[0] = std::make_unique<Rdt10Manager>();
	rdtSamples_[1] = std::make_unique<Rdt20Manager>();
	rdtSamples_[2] = std::make_unique<Rdt21Manager>();
	rdtSamples_[3] = std::make_unique<Rdt22Manager>();
	rdtSamples_[4] = std::make_unique<Rdt30Manager>();
	rdtSamples_[5] = std::make_unique<GoBackNManager>();
	rdtSamples_[6] = std::make_unique<SelRepManager>();
	rdtSamples_[currentIndex_]->Init();
	const auto defaultTxt = GetDefaultString();
	std::memcpy(inputTextBuffer_, defaultTxt.data(), defaultTxt.size()+1);
}

void HelloRdtProgram::Update(seconds dt)
{

	std::lock_guard<std::mutex> lock(updateMutex_);
	rdtSamples_[currentIndex_]->Update(dt);
	const auto receivedMsg = rdtSamples_[currentIndex_]->GetReceivedMsg();
	std::memcpy(outputTextBuffer_, receivedMsg.data(), receivedMsg.size()+1);
}

void HelloRdtProgram::Destroy()
{
	for(auto& sample: rdtSamples_)
	{
		if (sample)
		{
			sample->Destroy();
		}
	}
	
}

void HelloRdtProgram::DrawImGui()
{
	static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
	ImGui::Begin("Rdt Window");
	static const char* sampleNames[(int)RdtStatus::LENGTH] =
	{
		"Rdt 10",
		"Rdt 20",
		"Rdt 21",
		"Rdt 22",
		"Rdt 30",
		"Go Back N",
		"Selective Repeat"
	};
	const int currentSample = currentIndex_;
	if(ImGui::Combo("Rdt Sample", &currentIndex_, sampleNames, (int)RdtStatus::LENGTH))
	{
		rdtSamples_[currentSample]->Destroy();
		rdtSamples_[currentIndex_]->Init();
		currentStatus_ = static_cast<RdtStatus>(currentIndex_);
	}
	
	ImGui::InputTextMultiline("Input Text", inputTextBuffer_, IM_ARRAYSIZE(inputTextBuffer_), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
	if(ImGui::Button("Send"))
	{
		rdtSamples_[currentIndex_]->Send(inputTextBuffer_);
	}
	ImGui::InputTextMultiline("Output text", outputTextBuffer_, IM_ARRAYSIZE(outputTextBuffer_), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
	ImGui::End();
	rdtSamples_[currentIndex_]->DrawImGui();
}

void HelloRdtProgram::Render()
{

	std::lock_guard<std::mutex> lock(updateMutex_);
	rdtSamples_[currentIndex_]->Render();
}

void HelloRdtProgram::OnEvent(const SDL_Event& event)
{
	
}
}