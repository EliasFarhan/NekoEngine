#include <cmath>
#include <imgui.h>
#include <engine/engine.h>
#include "01_hello_triangle/triangle_program.h"
#include "engine/log.h"

namespace neko
{
void HelloTriangleProgram::Init()
{
    const auto& config = BasicEngine::GetInstance()->config;

    //Initiliaze Triangle
    triangleProgram_.shader.LoadFromFile(config.dataRootPath + "shaders/01_hello_triangle/hello_pos.vert",
                                         config.dataRootPath + "shaders/01_hello_triangle/hello_color.frag");

	glGenVertexArrays(1, &triangleProgram_.VAO);
    // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
    glBindVertexArray(triangleProgram_.VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
	glGenBuffers(1, &triangleProgram_.VBO); 
    glBindBuffer(GL_ARRAY_BUFFER, triangleProgram_.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleProgram_.vertices), &triangleProgram_.vertices, GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


	
    //Initiliaze VAO shader
    shader_.LoadFromFile(config.dataRootPath + "shaders/01_hello_triangle/hello_triangle.vert",
                         config.dataRootPath + "shaders/01_hello_triangle/hello_triangle.frag");

    //Initialize the VAO program
    glGenBuffers(1, &vaoProgam_.VBO);
    glGenVertexArrays(1, &vaoProgam_.VAO);
    // 1. bind Vertex Array Object
    glBindVertexArray(vaoProgam_.VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, vaoProgam_.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vaoProgam_.vertexData), vaoProgam_.vertexData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 2. copy our colors array in a buffer for OpenGL to use
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(18 * sizeof(float)));
    glEnableVertexAttribArray(1);


    //Initialize the EBO program
    glGenBuffers(2, &eboProgram_.VBO[0]);
    glGenBuffers(1, &eboProgram_.EBO);
    glGenVertexArrays(1, &eboProgram_.VAO);
    // 1. bind Vertex Array Object
    glBindVertexArray(eboProgram_.VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, eboProgram_.VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(eboProgram_.vertices), eboProgram_.vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    // 2. copy our colors array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, eboProgram_.VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(eboProgram_.colors), eboProgram_.colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboProgram_.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eboProgram_.indices), eboProgram_.indices, GL_STATIC_DRAW);

    nekoShader_.LoadFromFile(
            config.dataRootPath + "shaders/01_hello_triangle/hello_neko_quad.vert",
            config.dataRootPath + "shaders/01_hello_triangle/hello_triangle.frag");
    quad_.Init();
    circle_.Init();

    glEnable(GL_DEPTH_TEST);
    glCheckError();
}

void HelloTriangleProgram::Update(seconds dt)
{
    std::lock_guard<std::mutex> lock(updateMutex_);
    timeSinceInit_ += dt;
}

void HelloTriangleProgram::Render()
{
    if(shader_.GetProgram() == 0)
        return;
    std::lock_guard<std::mutex> lock(updateMutex_);
    switch (renderType_)
    {
    case RenderType::Triangle:
	    {
	        triangleProgram_.shader.Bind();
	        glBindVertexArray(triangleProgram_.VAO);
	        glDrawArrays(GL_TRIANGLES, 0, 3);
	        break;
	    }
        case RenderType::VaoProgram:
        {
            shader_.Bind();
            const float colorValue = (std::cos(timeSinceInit_.count()) + 1.0f) / 2.0f;
            shader_.SetFloat("colorCoeff", colorValue);

            glBindVertexArray(vaoProgam_.VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            break;
        }
        case RenderType::EboProgram:
        {
            shader_.Bind();
            const float colorValue = (std::cos(timeSinceInit_.count()) + 1.0f) / 2.0f;
            shader_.SetFloat("colorCoeff", colorValue);

            glBindVertexArray(eboProgram_.VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            break;
        }
        case RenderType::NekoQuad:
        {
            nekoShader_.Bind();
            const float colorValue = (std::cos(timeSinceInit_.count()) + 1.0f) / 2.0f;
            nekoShader_.SetFloat("colorCoeff", colorValue);
            nekoShader_.SetVec4("aColor", Vec4f(1.0f, 0.5f, 0.0f, 1.0f));
            quad_.Draw();

            break;
        }
        case RenderType::NekoCircle:
        {
            nekoShader_.Bind();
            const float colorValue = (std::cos(timeSinceInit_.count()) + 1.0f) / 2.0f;
            nekoShader_.SetFloat("colorCoeff", colorValue);
            nekoShader_.SetVec4("aColor", Vec4f(1.0f, 0.5f, 0.0f, 1.0f));
            circle_.Draw();
        }
        default:
            break;
    }


}

void HelloTriangleProgram::Destroy()
{
    glDeleteVertexArrays(1, &triangleProgram_.VAO);
    glDeleteBuffers(1, &triangleProgram_.VBO);
    triangleProgram_.shader.Destroy();
	
    glDeleteVertexArrays(1, &eboProgram_.VAO);
    glDeleteBuffers(2, &eboProgram_.VBO[0]);
    glDeleteBuffers(2, &eboProgram_.EBO);

    glDeleteVertexArrays(1, &vaoProgam_.VAO);
    glDeleteBuffers(1, &vaoProgam_.VBO);

    quad_.Destroy();
    circle_.Destroy();

    shader_.Destroy();
    nekoShader_.Destroy();


}

void HelloTriangleProgram::DrawImGui()
{

    ImGui::SetNextWindowPos(ImVec2(0, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Hello Triangle Program");
    const char* items[(size_t)RenderType::Length]= {
    		"Triangle Program",
            "Simple Vao Program",
            "Simple Vbo Program",
            "Neko Quad",
            "Neko Circle"
    };
    int currentIndex = static_cast<int>(renderType_);
    if(ImGui::Combo("Render Types", &currentIndex, items, (size_t)RenderType::Length))
    {
        renderType_ = static_cast<RenderType>(currentIndex);
    }
    ImGui::End();
}

void HelloTriangleProgram::OnEvent(const SDL_Event& event)
{

}

}
