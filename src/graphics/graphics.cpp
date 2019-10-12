/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

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
#include <graphics/graphics.h>
#include <algorithm>
#include <engine/globals.h>
#include "engine/log.h"


namespace neko
{
GraphicsManager::GraphicsManager()
{
	commandBuffer_.resize(MAX_COMMAND_NMB);
}



void GraphicsManager::Render()
{
    std::sort(commandBuffer_.begin(), commandBuffer_.begin() + renderLength_, [](GraphicsCommand* c1, GraphicsCommand* c2)
    {
        return c1->GetLayer() < c2->GetLayer();
    });
    for(Index i = 0; i < renderLength_;i++)
    {
        commandBuffer_[i]->Render();
    }
    commandBuffer_.clear();
    renderLength_ = 0;
}

void GraphicsManager::Draw(GraphicsCommand* command)
{
	if(renderLength_ >= MAX_COMMAND_NMB)
	{
		logDebug("[Error] Max Number of Graphics Command");
		return;
	}
    commandBuffer_[renderLength_] = command;
    renderLength_++;
}

int GraphicsCommand::GetLayer() const
{
    return layer_;
}

void GraphicsCommand::SetLayer(int layer)
{
    layer_ = layer;
}
}
