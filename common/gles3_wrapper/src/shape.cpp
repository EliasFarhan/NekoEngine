//
// Created by efarhan on 26.01.20.
//

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

    Vec3f tangent[4]{};
    for(int i = 0; i < 4; i++)
    {
        Vec3f edge1 = Vec3f(vertices[(i+1)%4]-vertices[i]);
        Vec3f edge2 = Vec3f(vertices[(i+2)%4]-vertices[i]);
        Vec2f deltaUV1 = texCoords[(i+1)%4]-texCoords[i];
        Vec2f deltaUV2 = texCoords[(i+2)%4]-texCoords[i];

        float f = 1.0f/(deltaUV1.x*deltaUV2.y-deltaUV2.x*deltaUV1.y);
        tangent[i].x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent[i].y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent[i].z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    }

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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2f), (void*) 0);
    glEnableVertexAttribArray(0);
    //bind texture coords data
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);
    // bind normals data
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3f), (void*)0);
    glEnableVertexAttribArray(2);
    // bind tangent data
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tangent), tangent, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3f), (void*)0);
    glEnableVertexAttribArray(3);
	//bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
    //Position 3 floats, texCoords 2 floats, Normal 3 floats 
    float vertices[] = {
    	//Right face 
         0.5f * size_.x + offset_.x,  0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f * size_.x + offset_.x, -0.5f * size_.y + offset_.y, -0.5f * size_.z + offset_.z,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
         0.5f * size_.x + offset_.x,  0.5f * size_.y + offset_.y, -0.5f * size_.z + offset_.z,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
         0.5f * size_.x + offset_.x, -0.5f * size_.y + offset_.y, -0.5f * size_.z + offset_.z,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
         0.5f * size_.x + offset_.x,  0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f * size_.x + offset_.x, -0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
         //Left face  
         -0.5f * size_.x + offset_.x,  0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
         -0.5f * size_.x + offset_.x,  0.5f * size_.y + offset_.y, -0.5f * size_.z + offset_.z,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
         -0.5f * size_.x + offset_.x, -0.5f * size_.y + offset_.y, -0.5f * size_.z + offset_.z,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
         -0.5f * size_.x + offset_.x, -0.5f * size_.y + offset_.y, -0.5f * size_.z + offset_.z,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
         -0.5f * size_.x + offset_.x, -0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
         -0.5f * size_.x + offset_.x,  0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
         //Top face   
         -0.5f * size_.x + offset_.x,  0.5f * size_.y + offset_.y, -0.5f * size_.z + offset_.z,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
          0.5f * size_.x + offset_.x,  0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
          0.5f * size_.x + offset_.x,  0.5f * size_.y + offset_.y, -0.5f * size_.z + offset_.z,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
          0.5f * size_.x + offset_.x,  0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
         -0.5f * size_.x + offset_.x,  0.5f * size_.y + offset_.y, -0.5f * size_.z + offset_.z,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
         -0.5f * size_.x + offset_.x,  0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
         //Bottom face              
         -0.5f * size_.x + offset_.x, -0.5f * size_.y + offset_.y, -0.5f * size_.z + offset_.z,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,
          0.5f * size_.x + offset_.x, -0.5f * size_.y + offset_.y, -0.5f * size_.z + offset_.z,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
          0.5f * size_.x + offset_.x, -0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
          0.5f * size_.x + offset_.x, -0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
         -0.5f * size_.x + offset_.x, -0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
         -0.5f * size_.x + offset_.x, -0.5f * size_.y + offset_.y, -0.5f * size_.z + offset_.z,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,
         //Front face  
         -0.5f * size_.x + offset_.x, -0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
          0.5f * size_.x + offset_.x, -0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
          0.5f * size_.x + offset_.x,  0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
          0.5f * size_.x + offset_.x,  0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
         -0.5f * size_.x + offset_.x,  0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
         -0.5f * size_.x + offset_.x, -0.5f * size_.y + offset_.y,  0.5f * size_.z + offset_.z,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
    	//Back face
        -0.5f*size_.x+offset_.x, -0.5f*size_.y+offset_.y, -0.5f*size_.z+offset_.z,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
         0.5f * size_.x + offset_.x,  0.5f * size_.y + offset_.y, -0.5f * size_.z + offset_.z,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
         0.5f*size_.x+offset_.x, -0.5f*size_.y+offset_.y, -0.5f*size_.z+offset_.z,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
         0.5f*size_.x+offset_.x,  0.5f*size_.y+offset_.y, -0.5f*size_.z+offset_.z,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
        -0.5f * size_.x + offset_.x, -0.5f * size_.y + offset_.y, -0.5f * size_.z + offset_.z,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
        -0.5f*size_.x+offset_.x,  0.5f*size_.y+offset_.y, -0.5f*size_.z+offset_.z,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
        
        
        
         
        
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO[0]);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void RenderCuboid::Draw() const
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void RenderCuboid::Destroy()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO[0]);
    //glDeleteBuffers(2, &EBO);
}

void RenderCircle::Init()
{
    Vec2f vertices[resolution+2];
    Vec2f texCoords[resolution+2];
    vertices[0] = Vec2f(offset_);
    texCoords[0] = Vec2f::one/2.0f;

    for(size_t i = 1; i < resolution+1; i++)
    {
        Vec2f vertex = Vec2f::up*radius_;
        auto angle = degree_t(360.0f/resolution* static_cast<float>(i-1));
        vertex = vertex.Rotate(angle);
        vertices[i] = vertex;

        texCoords[i] = Vec2f::one/2.0f+Vec2f::one/2.0f*Vec2f(Sin(angle), Cos(angle));
    }
    vertices[resolution+1] = vertices[1];
    texCoords[resolution+1] = texCoords[resolution+1];

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
}

void RenderCircle::Draw() const
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, resolution+2);
}

void RenderCircle::Destroy()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, &VBO[0]);
}
}