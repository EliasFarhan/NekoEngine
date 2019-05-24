#include <gtest/gtest.h>
#include <engine/engine.h>


TEST(Basic, Engine)
{
    neko::MainEngine engine;
    engine.Init();
    engine.EngineLoop();
}