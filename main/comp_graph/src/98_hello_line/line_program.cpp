#include "98_hello_line/line_program.h"
#include "graphics/graphics.h"

namespace neko
{
	void HelloLineProgram::Init()
	{
		lineRenderer_.Init();
		RendererLocator::get().RegisterSyncBuffersFunction(&lineRenderer_);
	}

	void HelloLineProgram::Update(seconds dt)
	{

		lineRenderer_.DrawLine(Line(-Vec3f::one, Vec3f::one));
		lineRenderer_.DrawLine(Line(Vec3f(1,-1,0), Vec3f(-1,1,0)));
		lineRenderer_.Update(dt);
	}

	void HelloLineProgram::Destroy()
	{
		lineRenderer_.Destroy();
	}

	void HelloLineProgram::DrawImGui()
	{
	}

	void HelloLineProgram::Render()
	{
		
	}

	void HelloLineProgram::OnEvent(const SDL_Event& event)
	{
	}
}
