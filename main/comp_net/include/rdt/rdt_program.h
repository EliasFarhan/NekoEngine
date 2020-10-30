#pragma once
#include "comp_net/sample_program.h"
#include "graphics/camera.h"
#include "rdt/rdt_base.h"

namespace neko::rdt
{
	
class HelloRdtProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	enum class RdtStatus
	{
		RDT_1_0,
		RDT_2_0,
		RDT_2_1,
		RDT_2_2,
		RDT_3_0,
		GO_BACK_N,
		SELECTIVE_REPEAT,
		LENGTH
	};
	std::array<std::unique_ptr<RdtManager>, (size_t)RdtStatus::LENGTH> rdtSamples_;
	int currentIndex_ = 0;
	RdtStatus currentStatus_ = RdtStatus::RDT_1_0;

	char inputTextBuffer_[1024]{};
	char outputTextBuffer_[1024]{};
};
}
