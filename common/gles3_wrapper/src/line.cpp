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

#include "gl/line.h"


#include "engine/engine.h"
#include "gl/gles3_include.h"

namespace neko::gl
{
void LineRenderer::Init()
{
	glGenVertexArrays(1, &vao_);
	glGenBuffers(2, &vbo_[0]);

	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	const auto& config = BasicEngine::GetInstance()->GetConfig();
    lineShader_.LoadFromFile(
            config.dataRootPath + "shaders/engine/line.vert",
            config.dataRootPath + "shaders/engine/line.frag");
}

void LineRenderer::Render()
{
	if (previousBuffer_.positions_.empty())
		return;
	const size_t lineNmb = previousBuffer_.positions_.size() / 2;
	//Generate the VAO
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
	glBufferData(GL_ARRAY_BUFFER, lineNmb * 2 * sizeof(Vec3f), &previousBuffer_.positions_[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
	glBufferData(GL_ARRAY_BUFFER, lineNmb * 2 * sizeof(Color3), &previousBuffer_.colors_[0], GL_DYNAMIC_DRAW);
	//Render the VAO
	lineShader_.Bind();
	glDrawArrays(GL_LINES, 0, lineNmb * 2);
}

void LineRenderer::Destroy()
{
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(2, &vbo_[0]);
	lineShader_.Destroy();
}
}
