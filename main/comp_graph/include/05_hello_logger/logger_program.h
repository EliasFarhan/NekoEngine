#pragma once

#include "comp_graph/sample_program.h"
#include "gl/shape.h"
#include "gl/shader.h"
#include "gl/texture.h"
#include "mathematics/vector.h"

namespace neko
{
class HelloLoggerProgram : public SampleProgram
{
public:
    void Init() override;

    void Update(seconds) override;

    void Render()override;

    void Destroy() override;

    void DrawImGui() override;

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