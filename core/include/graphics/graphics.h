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

#include <condition_variable>
#include <array>
#include <vector>
#include <thread>

#include "engine/system.h"
#include "engine/log.h"

namespace neko
{

const size_t MAX_COMMAND_NMB = 8'192;



/**
 * \brief abstraction of a graphic command send to the render thread
 */
class RenderCommandInterface
{
public:
    RenderCommandInterface() = default;
    virtual ~RenderCommandInterface() = default;
	virtual void Render() = 0;
};

/**
 * \brief Abstraction of a a graphic command that can be initialized, destroyed and update
 * Warning! Update can be run concurrently to Render!
 */
class RenderProgram : public RenderCommandInterface, public SystemInterface
{
};



class Renderer
{
public:
	Renderer();
	virtual ~Renderer() = default;
	void Render(RenderCommandInterface* command);
	void Sync();
	virtual void RenderAll();
	//TODO implement render loop
	void RenderLoop();
protected:
    std::thread renderThread_;
    std::vector<RenderCommandInterface*> currentCommandBuffer_ = {};
    std::vector<RenderCommandInterface*> nextCommandBuffer_ = {};
	size_t renderLength_ = 0;
};


}
