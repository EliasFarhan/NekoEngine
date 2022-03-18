/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

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

		lineRenderer_.DrawLine(Line(-Vec3f::one(), Vec3f::one()));
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
