#include "hello_triangle/triangle_command.h"
#include "engine/log.h"

namespace neko
{
void HelloTriangleCommand::Init()
{
	logDebug("Init Triangle Command");
	glGenBuffers(2, &VBO[0]);
	glGenBuffers(1, &EBO);

	shader_.CompileSource("data/shaders/hello_triangle/hello_triangle.vert", 
		"data/shaders/hello_triangle/hello_triangle.frag");

	glGenVertexArrays(1, &VAO);
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 2. copy our colors array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void HelloTriangleCommand::Update(float dt)
{

	timeSinceInit_ += dt;
}

void HelloTriangleCommand::Render()
{

	shader_.Bind();
	const float colorValue = (sin(timeSinceInit_) + 1.0f) / 2.0f;
	shader_.SetFloat("colorCoeff", colorValue);
	glUseProgram(shader_.GetProgram());

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void HelloTriangleCommand::Destroy()
{

	logDebug("Destroy Triangle Command");
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(2, &VBO[0]);
	glDeleteBuffers(2, &EBO);
}

}
