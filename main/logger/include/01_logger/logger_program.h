#pragma once
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
#include <logger/sample_program.h>
#include "graphics/graphics.h"

namespace neko
{
class LoggerProgram : public SampleProgram
{
public:

    void Init() override {}

    void Update(seconds dt) override {}
    void Render() override {}
    void Destroy() override {}

    void DrawUi() override;

    void OnEvent(const SDL_Event& event) override {}
	
    void GenerateTestLogs();

    static std::string LogCategoryToString(const LogCategories categories)
    {
        switch (categories)
        {
        default: return "All";
        case LogCategories::NONE: return "Not categorized";
        case LogCategories::ENGINE: return "Engine";
        case LogCategories::MATH: return "Math";
        case LogCategories::GRAPHICS: return "Graphics";
        case LogCategories::IO: return "I/O";
        case LogCategories::SOUND: return "Sound";
        }
    }

    static std::string LogTypeToString(const LogTypes types)
    {
        switch (types)
        {
        default: return "All";
        case LogTypes::DEBUG: return "Debug";
        case LogTypes::WARNING: return "Warning";
        case LogTypes::CRITICAL: return "Error";
        }
    }
private:
    //ImGui variables
    bool isActive_ = true;
	
    uint8_t currentCategories_ = (1 << static_cast<uint8_t>(LogCategories::LENGTH)) - 1;
    uint8_t currentTypes_ = (1 << static_cast<uint8_t>(LogTypes::LENGTH)) - 1;

    uint32_t logCount_ = 0;
};
}
