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

#include <City/city_command.h>
#include <City/city_zone.h>
#include "engine/globals.h"
#include "engine/engine.h"
#include "City/city_engine.h"
#include "City/city_editor.h"
#include "sound/sound.h"

namespace city
{
void CityCommandManager::AddCommand(std::unique_ptr<CityCommand> command, bool fromRenderThread)
{
	const neko::Index frameIndex = (multi::MainEngine::GetFrameIndex() - (fromRenderThread ? 1 : 0)) % 2;
	if (command->commandType == CityCommandType::CHANGE_CURSOR_MODE)
	{
		commandQueue_[frameIndex].insert(commandQueue_[frameIndex].begin(), std::move(command));
		for (auto i = 0u; i < commandQueue_[frameIndex].size(); i++)
		{
			if (commandQueue_[frameIndex][i]->commandType == CityCommandType::CREATE_CITY_ELEMENT)
			{
				commandQueue_[frameIndex].erase(commandQueue_[frameIndex].begin() + i);
				i--;
			}
		}
	}
	else
	{
		commandQueue_[(frameIndex + 1) % 2].push_back(std::move(command));
	}

}

void CityCommandManager::Init()
{
	soundBufferErase_ = neko::Sound::LoadSoundBuffer("data/Swip.wav");
	soundBufferBuild_ = neko::Sound::LoadSoundBuffer("data/Truip.wav");
	soundBufferRoad_ = neko::Sound::LoadSoundBuffer("data/Buip.wav");
	soundBufferSelect_ = neko::Sound::LoadSoundBuffer("data/Wiz.wav");
	soundBufferOut_ = neko::Sound::LoadSoundBuffer("data/Out.wav");
	soundErase_ = neko::Sound::CreateSoundFromBuffer(soundBufferErase_);
	soundBuild_ = neko::Sound::CreateSoundFromBuffer(soundBufferBuild_);
	soundRoad_ = neko::Sound::CreateSoundFromBuffer(soundBufferRoad_);
	soundSelect_ = neko::Sound::CreateSoundFromBuffer(soundBufferSelect_);
	soundOut_ = neko::Sound::CreateSoundFromBuffer(soundBufferOut_);
}

void CityCommandManager::ExecuteCommand(const std::shared_ptr<CityCommand>& command) const
{
	auto* engine = neko::BasicEngine::GetInstance<CityBuilderEngine>();
	switch (command->commandType)
	{
	case CityCommandType::CHANGE_CURSOR_MODE:
	{
		auto* cursorCommand = dynamic_cast<ChangeModeCommand*>(command.get());
		engine->GetCursor().SetCursorMode(cursorCommand->newCursorMode);
		neko::Sound::PlaySound(soundSelect_);
		break;
	}
	case CityCommandType::CREATE_CITY_ELEMENT:
	{
		if (engine->GetCityMoney() < roadCost)
		{
			neko::Sound::PlaySound(soundOut_);
			break;
		}
		auto* buildCommand = dynamic_cast<BuildElementCommand*>(command.get());
		engine->GetZoneManager().RemoveZone(buildCommand->position);
		engine->GetCityMap().AddCityElement(buildCommand->elementType, buildCommand->position);
		if (buildCommand->elementType == CityElementType::ROAD)
		{
			engine->ChangeCityMoney(-roadCost);
			engine->GetBuildingManager().RemoveBuilding(buildCommand->position);
		}
		neko::Sound::PlaySound(soundRoad_);
		break;
	}
	case CityCommandType::DELETE_CITY_ELEMENT:
	{
		auto* buildCommand = dynamic_cast<DestroyElementCommand*>(command.get());
		engine->GetCityMap().RemoveCityElement(buildCommand->position);
		engine->GetCarManager().RescheduleCarPathfinding(buildCommand->position);
		engine->GetZoneManager().RemoveZone(buildCommand->position);
		engine->GetBuildingManager().RemoveBuilding(buildCommand->position);
        neko::Sound::PlaySound(soundErase_);
		break;
	}
	case CityCommandType::ADD_CITY_ZONE:
	{
		auto* zoneCommand = dynamic_cast<AddZoneCommand*>(command.get());
		if (engine->GetCityMoney() < zoneCost)
		{
            neko::Sound::PlaySound(soundOut_);
		}
		engine->GetZoneManager().AddZone(zoneCommand->position, zoneCommand->zoneType, engine->GetCityMap());
        neko::Sound::PlaySound(soundBuild_);
		break;
	}
	case CityCommandType::CHANGE_TAX:
	{
		auto* taxCommand = dynamic_cast<ChangeTaxCommand*>(command.get());
		engine->houseTax = taxCommand->houseTax;
		engine->workTax = taxCommand->workTax;
	}
	default:
		break;
	}
}

void CityCommandManager::Update(float dt)
{
	const neko::Index frameIndex = (multi::MainEngine::GetFrameIndex()) % 2;
	const auto commandNmb = commandQueue_[frameIndex].size();
	for (auto i = 0u; i < commandNmb; i++)
	{
		auto command = commandQueue_[frameIndex][i];
		ExecuteCommand(command);
	}
	commandQueue_[frameIndex].clear();
}

void CityCommandManager::Destroy()
{
    neko::Sound::RemoveSoundBuffer(soundBufferErase_);
    neko::Sound::RemoveSoundBuffer(soundBufferBuild_);
    neko::Sound::RemoveSoundBuffer(soundBufferRoad_);
    neko::Sound::RemoveSoundBuffer(soundBufferSelect_);
}
}
