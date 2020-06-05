#pragma once
#include "comp_graph/sample_program.h"
#include "gl/line.h"

namespace neko
{
class HelloLineProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	gl::LineRenderer lineRenderer_;
};
}
