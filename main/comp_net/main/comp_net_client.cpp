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


#include <sdl_engine/sdl_engine.h>
#include <gl/gles3_window.h>
#include <gl/graphics.h>
#include "asteroid_net/network_client.h"

namespace neko::asteroid
{
class ClientEngine : public sdl::SdlEngine
{
public:
    explicit ClientEngine(Configuration* config = nullptr) : sdl::SdlEngine(config), client_()
    {
        RegisterOnDrawUi(client_);
        RegisterSystem(client_);
    }
	void Init() override
    {
        SdlEngine::Init();
        const auto& config = BasicEngine::GetInstance()->config;
        client_.SetWindowSize(config.windowSize);
    }
private:
    net::ClientNetworkManager client_;
};
}

int main(int argc, char** argv)
{
    neko::asteroid::ClientEngine engine;

    neko::sdl::Gles3Window window;
    neko::gl::Gles3Renderer renderer;

    engine.SetWindowAndRenderer(&window, &renderer);

    engine.Init();
    engine.EngineLoop();
    return EXIT_SUCCESS;
}