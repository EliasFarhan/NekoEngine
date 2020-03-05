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

    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void Render() override;

    void DrawUi() override;

    void OnEvent(const SDL_Event& event) override;


private:
    void RefreshLogs();
	
    const char* caterogies[7] = {
    	"NONE",
    	"ENGINE",
    	"MATH",
    	"GRAPHICS",
    	"IO",
    	"SOUND",
    	"ALL"
    };
	
    const char* types[4] = {
    	"DEBUG",
    	"WARNING",
    	"CRITICAL",
    	"ALL"
    };
	
    const char* logCount[5] = {
    	"50",
    	"100",
    	"150",
        "200",
        "ALL"
    };

    int maxLogCountSteps[5] = {
    	50,
    	100,
    	150,
    	200,
    	5000
    };
    int logScreenSizeSteps[5] = {
    	200,
    	250,
    	300,
    	350,
    	500
    };

    int selectedCat;
    int selectedType;
    int selectedLogCount;
    int logIndex;
    int maxLogToShow;
    int logScreenSize;
	
    std::vector<LogMessage> logsToShow;
    std::vector<LogMessage> currentSessionLogs;
	
    void InsertCurrentLog();
    void SetMaxLogsToShow();
	
    int CalculateCurrentLogIndex(int logCount, int currentY, int maxY);
	
	//TEST FUNCTIONS
    void WriteTestLog();
    void Write50Logs();
};
}
