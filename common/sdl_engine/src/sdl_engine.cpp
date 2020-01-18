/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

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
#include <sstream>
#include <mathematics/vector.h>
#include "sdl_engine/sdl_engine.h"
#include "engine/log.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#ifdef NEKO_GLES3
#include "imgui_impl_opengl3.h"
#endif

namespace neko::sdl
{
SdlEngine::SdlEngine(Configuration* config) : BasicEngine(config)
#ifdef NEKO_GLES3
, glContext_()
#endif
{
}

void SdlEngine::Init()
{
    BasicEngine::Init();
    SDL_Init(SDL_INIT_VIDEO);

    // Set our OpenGL version.
// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, config.glMajorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, config.glMinorVersion);


    // Turn on double buffering with a 24bit Z buffer.
    // You may need to change this to 16 or 32 for your system
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    auto flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    auto windowSize = config.realWindowSize;
    if(config.fullscreen)
    {
        int windowSizeW = 0;
        int windowSizeH = 0;
        SDL_GetWindowSize(window_, &windowSizeW , &windowSizeH);
        windowSize.first = windowSizeW;
        windowSize.second = windowSizeH;
        flags |= SDL_WINDOW_FULLSCREEN;
    }
    window_ = SDL_CreateWindow(
            config.windowName.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            windowSize.first,
            windowSize.second,
            flags
    );

    // Check that everything worked out okay
    if (window_ == nullptr)
    {
        logDebug("[Error] Unable to create window\n");
        return;
    }
#ifdef NEKO_GLES3
    glContext_ = SDL_GL_CreateContext(window_);

    const GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::ostringstream oss;
        oss << "[Error] while loading GLEW: " << glewGetErrorString(err);
        logDebug(oss.str());
        return;
    }
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void) io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window_, glContext_);
    ImGui_ImplOpenGL3_Init("#version 450");


    SDL_GL_SetSwapInterval(config.vSync);
#endif
	initDelegate_.Execute();
}

void SdlEngine::Update([[maybe_unused]]float dt)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {

        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
        {
            isRunning_ = false;
        }
        if (event.type == SDL_WINDOWEVENT)
        {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                {
                    std::ostringstream oss;
                    oss << "Window Size: " << event.window.data1 << ", " << event.window.data2;
                    logDebug(oss.str());
                }
                Vec2f newWindowSize = Vec2f(event.window.data1, event.window.data2);
                {
                    std::ostringstream oss;
                    oss << "New Window Size: " << newWindowSize;
                    logDebug(oss.str());
                }
                glViewport(0, 0, newWindowSize.x, newWindowSize.y);
                config.realWindowSize.first = event.window.data1;
                config.realWindowSize.second = event.window.data2;
            }
        }
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.scancode)
            {
                case SDL_SCANCODE_1:
                    //SwitchWireframeMode();
                    break;
                case SDL_SCANCODE_LALT:
                    //camera.SwitchWrapMode();
                    break;
                default:
                    break;
            }
        }
    }

	updateDelegate_.Execute(dt);
#ifdef NEKO_GLES3
    SDL_GL_MakeCurrent(window_, glContext_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (wireframeMode_)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
	drawDelegate_.Execute();

    if (wireframeMode_)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window_);
	ImGui::NewFrame();

	drawUiDelegate_.Execute(dt);

	ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (wireframeMode_)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    SDL_GL_SwapWindow(window_);
#endif
}

void SdlEngine::Destroy()
{
	destroyDelegate_.Execute();
#ifdef NEKO_GLES3
    ImGui_ImplOpenGL3_Shutdown();
#endif
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
#ifdef NEKO_GLES3
    // Delete our OpengL context
    SDL_GL_DeleteContext(glContext_);
#endif
    // Destroy our window
    SDL_DestroyWindow(window_);

    // Shutdown SDL 2
    SDL_Quit();

}
}
