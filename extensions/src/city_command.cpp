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

#include <city_command.h>
#include "engine/globals.h"
#include "engine/engine.h"
#include "city_engine.h"
#include "city_editor.h"

namespace neko
{
void CityCommandManager::AddCommand(std::unique_ptr<CityCommand> command, bool fromRenderThread)
{
	const Index frameIndex = (MainEngine::GetInstance()->frameIndex - (fromRenderThread ? 1 : 0)) % 2;
	commandQueue_[frameIndex].push(std::move(command));
}

void CityCommandManager::Init()
{
	engine_ = dynamic_cast<CityBuilderEngine*>(MainEngine::GetInstance());
}

void CityCommandManager::ExecuteCommand(const std::unique_ptr<CityCommand>& command) const
{
	switch (command->commandType)
	{
	case CityCommandType::CHANGE_CURSOR_MODE:
	{
		auto* cursorCommand = dynamic_cast<ChangeModeCommand*>(command.get());
		engine_->GetCursor().cursorMode = cursorCommand->newCursorMode;
		break;
	}
	case CityCommandType::CREATE_CITY_ELEMENT:
	{
		auto* buildCommand = dynamic_cast<BuildElementCommand*>(command.get());
		engine_->GetCityMap().AddCityElement(buildCommand->elementType, buildCommand->position);
		break;
	}
	case CityCommandType::DELETE_CITY_ELEMENT:
	{
		auto* buildCommand = dynamic_cast<DestroyElementCommand*>(command.get());
		engine_->GetCityMap().RemoveCityElement(buildCommand->position);
		engine_->GetCarManager().RescheduleCarPathfinding(buildCommand->position);
		break;
	}
	default:
		break;
	}
}

void CityCommandManager::Update()
{
	const Index frameIndex = (MainEngine::GetInstance()->frameIndex) % 2;
	while (!commandQueue_[frameIndex].empty())
	{
		auto command = std::move(commandQueue_[frameIndex].front());
		commandQueue_[frameIndex].pop();
		ExecuteCommand(command);
	}
}

void CityCommandManager::Destroy()
{
}
}
