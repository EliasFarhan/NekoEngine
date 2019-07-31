#include <tools/neko_editor.h>

int main(int argc, char** argv)
{
    editor::NekoEditor engine;
    engine.Init();
    engine.EngineLoop();
    return EXIT_SUCCESS;
}