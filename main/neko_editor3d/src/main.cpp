#include "editor_filesystem.h"
#include "model_viewer.h"

#include <editor_engine.h>
#include "gl/graphics.h"
#include "gl/gles3_window.h"


int main([[nodiscard]] int argc, [[nodiscard]] char** argv)
{
    neko::Configuration config;
    config.windowSize = neko::Vec2u(1280, 720);
    config.windowName = "Neko Editor";

    neko::sdl::Gles3Window window;
    neko::gl::Gles3Renderer renderer;
    neko::editor::EditorFilesystem editorFilesystem;
    neko::sdl::SdlEngine engine(editorFilesystem, config);
    neko::editor::Editor editor;
    engine.RegisterSystem(editor);
    engine.RegisterOnEvent(editor);
    engine.RegisterOnDrawUi(editor);


    engine.SetWindowAndRenderer(&window, &renderer);
    engine.Init();
    engine.EngineLoop();
    return 0;
}
