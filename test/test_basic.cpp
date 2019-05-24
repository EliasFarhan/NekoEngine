#include <gtest/gtest.h>
#include <engine/engine.h>


TEST(Basic, Engine)
{
    neko::Engine engine;
    engine.Init();
    engine.EngineLoop();
}