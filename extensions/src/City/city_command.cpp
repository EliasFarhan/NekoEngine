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
	soundBufferErase_ = Sound::LoadSoundBuffer("data/Swip.wav");
	soundBufferBuild_ = Sound::LoadSoundBuffer("data/Truip.wav");
	soundBufferRoad_ = Sound::LoadSoundBuffer("data/Buip.wav");
	soundBufferSelect_ = Sound::LoadSoundBuffer("data/Wiz.wav");
	soundErase_ = Sound::CreateSoundFromBuffer(soundBufferErase_);
	soundBuild_ = Sound::CreateSoundFromBuffer(soundBufferBuild_);
	soundRoad_ = Sound::CreateSoundFromBuffer(soundBufferRoad_);
	soundSelect_ = Sound::CreateSoundFromBuffer(soundBufferSelect_);
}

void CityCommandManager::ExecuteCommand(const std::unique_ptr<CityCommand>& command) const
{
    switch (command->commandType)
    {
        case CityCommandType::CHANGE_CURSOR_MODE:
        {
            auto* cursorCommand = dynamic_cast<ChangeModeCommand*>(command.get());
            engine_->GetCursor().cursorMode = cursorCommand->newCursorMode;
			Sound::PlaySound(soundSelect_);
            break;
        }
        case CityCommandType::CREATE_CITY_ELEMENT:
        {
            auto* buildCommand = dynamic_cast<BuildElementCommand*>(command.get());
            engine_->GetZoneManager().RemoveZone(buildCommand->position);
            engine_->GetCityMap().AddCityElement(buildCommand->elementType, buildCommand->position);
            if(buildCommand->elementType == CityElementType::ROAD)
            {
                engine_->GetBuildingManager().RemoveBuilding(buildCommand->position);
            }
			Sound::PlaySound(soundRoad_);
            break;
        }
        case CityCommandType::DELETE_CITY_ELEMENT:
        {
            auto* buildCommand = dynamic_cast<DestroyElementCommand*>(command.get());
            engine_->GetCityMap().RemoveCityElement(buildCommand->position);
            engine_->GetCarManager().RescheduleCarPathfinding(buildCommand->position);
            engine_->GetZoneManager().RemoveZone(buildCommand->position);
            engine_->GetBuildingManager().RemoveBuilding(buildCommand->position);
			Sound::PlaySound(soundErase_);
            break;
        }
        case CityCommandType::ADD_CITY_ZONE:
        {
            auto* zoneCommand = dynamic_cast<AddZoneCommand*>(command.get());
            engine_->GetZoneManager().AddZone(zoneCommand->position, zoneCommand->zoneType, engine_->GetCityMap());
			Sound::PlaySound(soundBuild_);
			break;
        }
        default:
            break;
    }
}

void CityCommandManager::Update(float dt)
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
	Sound::RemoveSoundBuffer(soundBufferErase_);
	Sound::RemoveSoundBuffer(soundBufferBuild_);
	Sound::RemoveSoundBuffer(soundBufferRoad_);
	Sound::RemoveSoundBuffer(soundBufferSelect_);
}
}
