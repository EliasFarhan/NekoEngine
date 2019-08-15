#include <tools/neko_editor.h>
#include<iostream>

int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv)
{
    editor::NekoEditor engine;
    engine.Init();
    engine.EngineLoop();
    return EXIT_SUCCESS;
}