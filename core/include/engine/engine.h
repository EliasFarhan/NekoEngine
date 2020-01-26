#pragma once

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

#include <string>
#include <engine/system.h>
#include <utilities/action_utility.h>
#include <graphics/color.h>
#include <utilities/time_utility.h>
#include <mathematics/vector.h>


namespace neko
{
struct Collider;

/**
 * \brief store various Engine constant or global values
 */
struct Configuration
{
	std::string windowName = "NekoEngine 0.1";
    Vec2i windowSize = Vec2i(1280, 720);
    std::pair<unsigned, unsigned> gameWindowSize = std::pair<unsigned, unsigned>(1280, 720);
    bool fullscreen = false;
    bool vSync = true;
    unsigned int framerateLimit = 0u;
    std::string dataRootPath = "data/";
};


/**
 * \brief basic engine class with no graphics manager implementation
 */
class BasicEngine : public System
{
public:
    explicit BasicEngine(Configuration* config = nullptr);
	BasicEngine() = delete;
    ~BasicEngine() override;
    void Init() override;

    void Destroy() override;

    void EngineLoop();


    Configuration config;

    static BasicEngine* GetInstance(){return instance_;};
    //template <typename T = BasicEngine>
    //static T* GetInstance(){ return dynamic_cast<T*>(instance_);};
protected:
    static BasicEngine* instance_;
	bool isRunning_;
    Action<> initAction_;
    Action<seconds> updateAction_;
    Action<seconds> drawUiAction_;
    Action<> drawAction_;
    Action<> destroyAction_;

};

}
