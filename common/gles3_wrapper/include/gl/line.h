#pragma once
#include "gl/shader.h"
#include "graphics/line.h"

namespace neko::gl
{
class LineRenderer : public neko::LineRenderer
{
public:
	void Init() override;
	void Render() override;
	void Destroy() override;
protected:
	unsigned int vao_ = 0;
	unsigned int vbo_[2]{};
	Shader lineShader_;
};
}
