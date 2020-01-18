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

#include "engine/system.h"
#include "engine/log.h"

namespace neko
{

const size_t MAX_COMMAND_NMB = 8'192;


struct RenderTarget
{
	void* renderTargetPtr = nullptr;
};

/**
 * \brief abstraction of a graphic command send to the render thread
 */
class RenderCommand
{
public:
    RenderCommand() = default;
    virtual ~RenderCommand() = default;

    int GetLayer() const;
    void SetLayer(int layer);


	virtual void Render(RenderTarget* renderTarget) = 0;
private:
    int layer_ = 0;

};

class RenderProgram : public RenderCommand
{
public:
    virtual void Init() = 0;
    virtual void Update(float dt) = 0;
    virtual void Destroy() = 0;
};



class GraphicsManager
{
public:

	GraphicsManager();
	virtual ~GraphicsManager() = default;

	void Clear(){};
	void Render(RenderCommand* command);
	virtual void RenderAll(RenderTarget* renderTarget = nullptr);
protected:
    std::vector<RenderCommand*> commandBuffer_ = {};
	size_t renderLength_ = 0;
};


}
