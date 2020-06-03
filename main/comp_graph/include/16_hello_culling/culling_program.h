#pragma once
#include "comp_graph/sample_program.h"
#include "gl/shader.h"
#include "gl/model.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{

class HelloCullingProgram : public SampleProgram
{
public:
private:
	sdl::Camera3D camera_;
	gl::Shader modelShader_;

	sdl::Camera3D externalCamera_;
};

}
