//
// Created by efarhan on 25.01.20.
//
#include "gl/gles3_window.h"
#include "comp_graph/comp_graph_engine.h"


int main(int argc, char** argv)
{
    neko::sdl::Gles3Window window;
    neko::CompGraphEngine engine;
    engine.Init();
    engine.EngineLoop();
    return 0;
}