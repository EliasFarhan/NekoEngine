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

    float texCoords[8] = {
            1.0f, 1.0f,      // top right
            1.0f, 0.0f,   // bottom right
            0.0f, 0.0f,   // bottom left
            0.0f, 1.0f,   // bottom left
    };

    unsigned int indices[6] = {
            // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };

    //Initialize the EBO program
    glGenBuffers(2, &VBO[0]);
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
    glDeleteBuffers(2, &VBO[0]);
    glDeleteBuffers(2, &EBO);

}

void RenderCuboid::Init()
{

    Vec3f vertices[8] =
    {
        Vec3f(-0.5f, -0.5f, -0.5f) * size_ + offset_,//0
        Vec3f(0.5f, -0.5f, -0.5f) * size_ + offset_,//1
        Vec3f(0.5f, 0.5f, -0.5f) * size_ + offset_,//2
        //2 Vec3f( 0.5f,  0.5f, -0.5f)
        Vec3f(-0.5f, 0.5f, -0.5f) * size_ + offset_,//3
        //0 Vec3f(-0.5f, -0.5f, -0.5f),

        Vec3f(-0.5f, -0.5f, 0.5f) * size_ + offset_,//4
        Vec3f(0.5f, -0.5f, 0.5f) * size_ + offset_,//5
        Vec3f(0.5f, 0.5f, 0.5f) * size_ + offset_,//6
        //7 Vec3f( 0.5f,  0.5f,  0.5f),
        Vec3f(-0.5f, 0.5f, 0.5f) * size_ + offset_,//7
        //5 Vec3f(-0.5f, -0.5f,  0.5f),//9

        //8 Vec3f(-0.5f,  0.5f,  0.5f),
        //3 Vec3f(-0.5f,  0.5f, -0.5f),
        //0 Vec3f(-0.5f, -0.5f, -0.5f),
        //0 Vec3f(-0.5f, -0.5f, -0.5f),
        //5 Vec3f(-0.5f, -0.5f,  0.5f),
        //8 Vec3f(-0.5f,  0.5f,  0.5f),

        //7 Vec3f( 0.5f,  0.5f,  0.5f),
        //2 Vec3f( 0.5f,  0.5f, -0.5f),
        //1 Vec3f( 0.5f, -0.5f, -0.5f),
        //1 Vec3f( 0.5f, -0.5f, -0.5f),
        //6 Vec3f( 0.5f, -0.5f,  0.5f),
        //7 Vec3f( 0.5f,  0.5f,  0.5f),

        //0 Vec3f(-0.5f, -0.5f, -0.5f),
        //1 Vec3f( 0.5f, -0.5f, -0.5f),
        //6 Vec3f( 0.5f, -0.5f,  0.5f),
        //6 Vec3f( 0.5f, -0.5f,  0.5f),
        //5 Vec3f(-0.5f, -0.5f,  0.5f),
        //0 Vec3f(-0.5f, -0.5f, -0.5f),

        //3 Vec3f(-0.5f,  0.5f, -0.5f),
        //2 Vec3f( 0.5f,  0.5f, -0.5f),
        //7 Vec3f( 0.5f,  0.5f,  0.5f),
        //7 Vec3f( 0.5f,  0.5f,  0.5f),
        //8 Vec3f(-0.5f,  0.5f,  0.5f),
        //3 Vec3f(-0.5f,  0.5f, -0.5f)
    };

    float texCoords[16] =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };
    unsigned int indices[36] =
    {
        0, 1, 2,
        2, 3, 0,
        4, 6, 5,
        4, 7, 6,
        7, 0, 3,
        0, 7, 4,
        6, 2, 1,
        1, 5, 6,
        0, 1, 5,
        5, 4, 0,
        3, 6, 2,
        6, 3, 7
    };
    //Initialize the EBO program
    glGenBuffers(2, &VBO[0]);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3f), (void*) nullptr);
    glEnableVertexAttribArray(0);
    //bind texture coords data
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) nullptr);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void RenderCuboid::Draw() const
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void RenderCuboid::Destroy()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, &VBO[0]);
    glDeleteBuffers(2, &EBO);
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