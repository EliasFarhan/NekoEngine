#pragma once

#include "graphics/graphics.h"
#include "gl_engine/gl_shader.h"

namespace neko
{
class HelloTriangleCommand : public GraphicsCommand
{
public:
	void Init() override;
	void Update(float dt) override;
	void Destroy() override;
	void Render() override;
private:
	float vertices[12] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	float colors[12] = {
		1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.5f, 0.0f, 0.5f
	};
	unsigned int indices[6] = {
		// note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	unsigned int EBO = 0; // Element Buffer Object
	unsigned int VBO[2] = {}; //Vertex Buffer Object
	unsigned int VAO = 0; //Vertex Array Object
	float timeSinceInit_ = 0.0f;
	gl::Shader shader_;
};
}
