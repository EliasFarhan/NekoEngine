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

#include <gl/shape.h>
#include <mathematics/trigo.h>
#include "gl/gles3_include.h"

namespace neko::gl
{


void RenderQuad::Init()
{
	Vec2f vertices[4] = {
			Vec2f(0.5f, 0.5f) * size_ + Vec2f(offset_),  // top right
			Vec2f(0.5f, -0.5f) * size_ + Vec2f(offset_),  // bottom right
			Vec2f(-0.5f, -0.5f) * size_ + Vec2f(offset_),  // bottom left
			Vec2f(-0.5f, 0.5f) * size_ + Vec2f(offset_)   // top left
	};

	Vec2f texCoords[4] = {
			Vec2f(1.0f, 1.0f),      // top right
			Vec2f(1.0f, 0.0f),   // bottom right
			Vec2f(0.0f, 0.0f),   // bottom left
			Vec2f(0.0f, 1.0f),   // bottom left
	};

	Vec3f normals[4] = {
		Vec3f::back,
		Vec3f::back,
		Vec3f::back,
		Vec3f::back
	};

	std::array<Vec3f, 4> tangent{};

	{
		const Vec3f edge1 = Vec3f(vertices[1] - vertices[0]);
		const Vec3f edge2 = Vec3f(vertices[2] - vertices[0]);
		const Vec2f deltaUV1 = texCoords[1] - texCoords[0];
		const Vec2f deltaUV2 = texCoords[2] - texCoords[0];

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		tangent[0].x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent[0].y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent[0].z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	}
	std::fill(tangent.begin() + 1, tangent.end(), tangent[0]);

	unsigned int indices[6] = {
		// note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	//Initialize the EBO program
	glGenBuffers(4, &VBO[0]);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2f), (void*)0);
	glEnableVertexAttribArray(0);
	//bind texture coords data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	// bind normals data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3f), (void*)0);
	glEnableVertexAttribArray(2);
	// bind tangent data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tangent), &tangent[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3f), (void*)0);
	glEnableVertexAttribArray(3);
	//bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glCheckError();
}

void RenderQuad::Draw() const
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void RenderQuad::Destroy()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(4, &VBO[0]);
	glDeleteBuffers(1, &EBO);

}

void RenderCuboid::Init()
{
	Vec3f position[36] =
	{
		//Right face 
		 Vec3f(0.5f,   0.5f ,  0.5f) * size_ + offset_,
		 Vec3f(0.5f,  -0.5f , -0.5f) * size_ + offset_,
		 Vec3f(0.5f,   0.5f , -0.5f) * size_ + offset_,
		 Vec3f(0.5f,  -0.5f , -0.5f) * size_ + offset_,
		 Vec3f(0.5f,   0.5f ,  0.5f) * size_ + offset_,
		 Vec3f(0.5f,  -0.5f ,  0.5f) * size_ + offset_,
		 //Left face                 *size_+offset_
		 Vec3f(-0.5f,  0.5f ,  0.5f) * size_ + offset_,
		 Vec3f(-0.5f,  0.5f , -0.5f) * size_ + offset_,
		 Vec3f(-0.5f, -0.5f , -0.5f) * size_ + offset_,
		 Vec3f(-0.5f, -0.5f , -0.5f) * size_ + offset_,
		 Vec3f(-0.5f, -0.5f ,  0.5f) * size_ + offset_,
		 Vec3f(-0.5f,  0.5f ,  0.5f) * size_ + offset_,
		 //Top face                  *size_+offset_
		 Vec3f(-0.5f,  0.5f , -0.5f) * size_ + offset_,
		 Vec3f(0.5f,  0.5f ,  0.5f) * size_ + offset_,
		 Vec3f(0.5f,  0.5f , -0.5f) * size_ + offset_,
		 Vec3f(0.5f,  0.5f ,  0.5f) * size_ + offset_,
		 Vec3f(-0.5f,  0.5f , -0.5f) * size_ + offset_,
		 Vec3f(-0.5f,  0.5f ,  0.5f) * size_ + offset_,
		 //Bottom fa                 *size_+offset_
		 Vec3f(-0.5f, -0.5f , -0.5f) * size_ + offset_,
		 Vec3f(0.5f, -0.5f , -0.5f) * size_ + offset_,
		 Vec3f(0.5f, -0.5f ,  0.5f) * size_ + offset_,
		 Vec3f(0.5f, -0.5f ,  0.5f) * size_ + offset_,
		 Vec3f(-0.5f, -0.5f ,  0.5f) * size_ + offset_,
		 Vec3f(-0.5f, -0.5f , -0.5f) * size_ + offset_,
		 //Front fac                 *size_+offset_
		 Vec3f(-0.5f, -0.5f ,  0.5f) * size_ + offset_,
		 Vec3f(0.5f, -0.5f ,  0.5f) * size_ + offset_,
		 Vec3f(0.5f,  0.5f ,  0.5f) * size_ + offset_,
		 Vec3f(0.5f,  0.5f ,  0.5f) * size_ + offset_,
		 Vec3f(-0.5f,  0.5f ,  0.5f) * size_ + offset_,
		 Vec3f(-0.5f, -0.5f ,  0.5f) * size_ + offset_,
		 //Back face
		Vec3f(-0.5f , -0.5f , -0.5f )*size_+offset_,
		Vec3f( 0.5f ,  0.5f , -0.5f )*size_+offset_,
		Vec3f( 0.5f , -0.5f , -0.5f )*size_+offset_,
		Vec3f( 0.5f ,  0.5f , -0.5f )*size_+offset_,
		Vec3f(-0.5f , -0.5f , -0.5f )*size_+offset_,
		Vec3f(-0.5f ,  0.5f , -0.5f )*size_+offset_,
	};
	Vec2f texCoords[36] = {
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 0.0f),
		
		Vec2f(1.0f, 0.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 0.0f),
		
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(0.0f, 0.0f),
		
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(0.0f, 1.0f),
		
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(0.0f, 0.0f),
		
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(0.0f, 1.0f),
	};

	Vec3f normals[36] =
	{
		Vec3f(1.0f, 0.0f, 0.0f),
		Vec3f(1.0f, 0.0f, 0.0f),
		Vec3f(1.0f, 0.0f, 0.0f),
		Vec3f(1.0f, 0.0f, 0.0f),
		Vec3f(1.0f, 0.0f, 0.0f),
		Vec3f(1.0f, 0.0f, 0.0f),
		
		Vec3f(-1.0f, 0.0f, 0.0f),
		Vec3f(-1.0f, 0.0f, 0.0f),
		Vec3f(-1.0f, 0.0f, 0.0f),
		Vec3f(-1.0f, 0.0f, 0.0f),
		Vec3f(-1.0f, 0.0f, 0.0f),
		Vec3f(-1.0f, 0.0f, 0.0f),
		
		Vec3f(0.0f, 1.0f, 0.0f),
		Vec3f(0.0f, 1.0f, 0.0f),
		Vec3f(0.0f, 1.0f, 0.0f),
		Vec3f(0.0f, 1.0f, 0.0f),
		Vec3f(0.0f, 1.0f, 0.0f),
		Vec3f(0.0f, 1.0f, 0.0f),
		
		Vec3f(0.0f, -1.0f, 0.0f),
		Vec3f(0.0f, -1.0f, 0.0f),
		Vec3f(0.0f, -1.0f, 0.0f),
		Vec3f(0.0f, -1.0f, 0.0f),
		Vec3f(0.0f, -1.0f, 0.0f),
		Vec3f(0.0f, -1.0f, 0.0f),
		
		Vec3f(0.0f, 0.0f, 1.0f),
		Vec3f(0.0f, 0.0f, 1.0f),
		Vec3f(0.0f, 0.0f, 1.0f),
		Vec3f(0.0f, 0.0f, 1.0f),
		Vec3f(0.0f, 0.0f, 1.0f),
		Vec3f(0.0f, 0.0f, 1.0f),
		
		Vec3f(0.0f, 0.0f, -1.0f),
		Vec3f(0.0f, 0.0f, -1.0f),
		Vec3f(0.0f, 0.0f, -1.0f),
		Vec3f(0.0f, 0.0f, -1.0f),
		Vec3f(0.0f, 0.0f, -1.0f),
		Vec3f(0.0f, 0.0f, -1.0f),
	};

	Vec3f tangent[36]{};
	for(int i = 0; i < 36; i+=3)
	{
		const Vec3f edge1 = position[i+1] - position[i];
		const Vec3f edge2 = position[i+2] - position[i];
		const Vec2f deltaUV1 = texCoords[i+1] - texCoords[i];
		const Vec2f deltaUV2 = texCoords[i+2] - texCoords[i];

		const float f = 1.0f / (deltaUV1.u * deltaUV2.v - deltaUV2.u * deltaUV1.v);
		tangent[i].x = f * (deltaUV2.v * edge1.x - deltaUV1.v * edge2.x);
		tangent[i].y = f * (deltaUV2.v * edge1.y - deltaUV1.v * edge2.y);
		tangent[i].z = f * (deltaUV2.v * edge1.z - deltaUV1.v * edge2.z);
		tangent[i + 1] = tangent[i];
		tangent[i + 2] = tangent[i];
	}
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(4, &VBO[0]);

	glBindVertexArray(VAO);
	// position attribute
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glEnableVertexAttribArray(1);
	// normal attribute
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(2);
	//tangent attribute
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tangent), tangent, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
	glCheckError();
}

void RenderCuboid::Draw() const
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void RenderCuboid::Destroy()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(4, &VBO[0]);
	//glDeleteBuffers(2, &EBO);
}

RenderSphere::RenderSphere(Vec3f offset, float radius, size_t segment) : neko::RenderSphere(offset, radius), segment_(segment)
{
}

void RenderSphere::Init()
{
	glCheckError();
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO[0]);
	glGenBuffers(1, &EBO);

	std::vector<Vec3f> positions;
	positions.reserve((segment_ + 1) * (segment_ + 1));
	std::vector<Vec2f> uv;
	uv.reserve((segment_ + 1) * (segment_ + 1));
	std::vector<Vec3f> normals;
	normals.reserve((segment_ + 1) * (segment_ + 1));
	std::vector<Vec3f> tangent;
	tangent.resize((segment_+1) * (segment_+1));
	std::vector<unsigned int> indices;
	indices.reserve((segment_ + 1) * segment_);
	for (unsigned int y = 0; y <= segment_; ++y)
	{
		for (unsigned int x = 0; x <= segment_; ++x)
		{
			float xSegment = static_cast<float>(x) / static_cast<float>(segment_);
			float ySegment = static_cast<float>(y) / static_cast<float>(segment_);
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			positions.emplace_back(xPos, yPos, zPos);
			uv.emplace_back(xSegment, ySegment);
			normals.emplace_back(xPos, yPos, zPos);
		}
	}

	bool oddRow = false;
	for (unsigned int y = 0; y < segment_; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (unsigned int x = 0; x <= segment_; ++x)
			{
				indices.push_back(y * (segment_ + 1) + x);
				indices.push_back((y + 1) * (segment_ + 1) + x);
			}
		}
		else
		{
			for (int x = segment_; x >= 0; --x)
			{
				indices.push_back((y + 1) * (segment_ + 1) + x);
				indices.push_back(y * (segment_ + 1) + x);
			}
		}
		oddRow = !oddRow;
	}
	indexCount_ = indices.size();
	for (size_t i = 0; i < indexCount_ - 2; i++)
	{
		const Vec3f edge1 = positions[indices[i + 1]] - positions[indices[i]];
		const Vec3f edge2 = positions[indices[i + 2]] - positions[indices[i]];
		const Vec2f deltaUV1 = uv[indices[i + 1]] - uv[indices[i]];
		const Vec2f deltaUV2 = uv[indices[i + 2]] - uv[indices[i]];

		const float f = 1.0f / (deltaUV1.u * deltaUV2.v - deltaUV2.u * deltaUV1.v);
		tangent[indices[i]].x = f * (deltaUV2.v * edge1.x - deltaUV1.v * edge2.x);
		tangent[indices[i]].y = f * (deltaUV2.v * edge1.y - deltaUV1.v * edge2.y);
		tangent[indices[i]].z = f * (deltaUV2.v * edge1.z - deltaUV1.v * edge2.z);

	}

	std::vector<float> data;
	data.reserve(positions.size() * sizeof(Vec3f) + uv.size() * sizeof(Vec2f) + normals.size() * sizeof(Vec3f));
	for (unsigned int i = 0; i < positions.size(); ++i)
	{
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);
		if (!uv.empty())
		{
			data.push_back(uv[i].x);
			data.push_back(uv[i].y);
		}
		if (!normals.empty())
		{
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}
		if(!tangent.empty())
		{
			data.push_back(tangent[i].x);
			data.push_back(tangent[i].y);
			data.push_back(tangent[i].z);
		}
	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	const auto stride = (3 + 2 + 3 + 3) * sizeof(float);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float)));
	glBindVertexArray(0);
	glCheckError();
}

void RenderSphere::Draw() const
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount_, GL_UNSIGNED_INT, 0);
}

void RenderSphere::Destroy()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO[0]);
	glDeleteBuffers(1, &EBO);
}

void RenderCircle::Init()
{
	glCheckError();
	Vec2f vertices[resolution + 2];
	Vec2f texCoords[resolution + 2];
	vertices[0] = Vec2f(offset_);
	texCoords[0] = Vec2f::one / 2.0f;

	for (size_t i = 1; i < resolution + 1; i++)
	{
		Vec2f vertex = Vec2f::up * radius_;
		auto angle = degree_t(360.0f / resolution * static_cast<float>(i - 1));
		vertex = vertex.Rotate(angle);
		vertices[i] = vertex;

		texCoords[i] = Vec2f::one / 2.0f + Vec2f::one / 2.0f * Vec2f(Sin(angle), Cos(angle));
	}
	vertices[resolution + 1] = vertices[1];
	texCoords[resolution + 1] = texCoords[resolution + 1];

	//Initialize the EBO program
	glGenBuffers(2, &VBO[0]);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2f), (void*) nullptr);
	glEnableVertexAttribArray(0);
	//bind texture coords data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2f), (void*) nullptr);
	glBindVertexArray(0);
	glCheckError();
}

void RenderCircle::Draw() const
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, resolution + 2);
}

void RenderCircle::Destroy()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(2, &VBO[0]);
}
}