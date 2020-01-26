//
// Created by efarhan on 26.01.20.
//

#include <gl/shape.h>
#include "gl/gles3_include.h"

namespace neko::gl
{
void Quad::Init()
{
    Vec2f vertices[4] = {
            Vec2f(0.5f, 0.5f)*size_-origin_,  // top right
            Vec2f(0.5f, -0.5f)*size_-origin_,  // bottom right
            Vec2f(-0.5f, -0.5f)*size_-origin_,  // bottom left
            Vec2f(-0.5f, 0.5f)*size_-origin_   // top left
    };

    float texCoords[8] = {
            1.0f, 1.0f,	  // top right
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
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void Quad::Draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Quad::Destroy()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, &VBO[0]);
    glDeleteBuffers(2, &EBO);

}

Quad::Quad(Vec2f origin, Vec2f size) : origin_(origin), size_(size)
{

}
}