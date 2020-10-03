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

#include <gl/graphics.h>
#include <gl/gles3_window.h>
#include "sdl_engine/sdl_engine.h"
#include "asteroid_simulation/asteroid_debug_app.h"

namespace neko::asteroid
{
class DebugEngine : public sdl::SdlEngine
{
public:
    explicit DebugEngine(Configuration* config = nullptr) : sdl::SdlEngine(config)
    {
        RegisterSystem(app_);
        RegisterOnDrawUi(app_);
        RegisterOnEvent(app_);
    }

    void Destroy() override
    {
        sdl::SdlEngine::Destroy();
        app_.Destroy();
    }

private:
    net::AsteroidDebugApp app_;
};
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    neko::asteroid::DebugEngine engine;

    neko::sdl::Gles3Window window;
    neko::gl::Gles3Renderer renderer;

    engine.SetWindowAndRenderer(&window, &renderer);

    engine.Init();
    engine.EngineLoop();
    return EXIT_SUCCESS;
}