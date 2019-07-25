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

#include <queue>
#include <memory>
#include "engine/system.h"
#include "engine/globals.h"

namespace city
{
class CityBuilderEngine;
enum class CityCommandType
{
	CHANGE_CURSOR_MODE,
	CHANGE_TAX,
	CREATE_CITY_ELEMENT,
	DELETE_CITY_ELEMENT,
	ADD_CITY_ZONE,
	REMOVE_CITY_ZONE,
	NONE
};

struct CityCommand
{
	CityCommand() = default;
	virtual ~CityCommand() = default;
	CityCommandType commandType = CityCommandType::NONE;
};

class CityCommandManager : public neko::System
{
	
public:
	void Init() override;
	void ExecuteCommand(const std::shared_ptr<CityCommand>& command) const;
	void Update(float dt) override;
	void Destroy() override;
	void AddCommand(std::unique_ptr<CityCommand> command, bool fromRenderThread = false);

protected:
	std::vector<std::shared_ptr<CityCommand>>commandQueue_[2];
    neko::Index soundErase_ = neko::INVALID_INDEX;
    neko::Index soundBuild_ = neko::INVALID_INDEX;
    neko::Index soundRoad_ = neko::INVALID_INDEX;
    neko::Index soundSelect_ = neko::INVALID_INDEX;
    neko::Index soundBufferOut_ = neko::INVALID_INDEX;
    neko::Index soundBufferErase_ = neko::INVALID_INDEX;
    neko::Index soundBufferBuild_ = neko::INVALID_INDEX;
    neko::Index soundBufferRoad_ = neko::INVALID_INDEX;
    neko::Index soundBufferSelect_ = neko::INVALID_INDEX;
    neko::Index soundOut_ = neko::INVALID_INDEX;
};
}
