#include "editor_filesystem.h"
#include "model_viewer.h"

#include <editor_engine.h>
#include "gl/graphics.h"
#include "gl/sdl_window.h"


int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    neko::gl::Gles3Window window;
    neko::gl::Gles3Renderer renderer;
    neko::editor::EditorFilesystem editorFilesystem;
    neko::sdl::SdlEngine engine(editorFilesystem);
    auto& config = engine.MutableConfig();
    config.set_window_size_x(1280);
    config.set_window_size_y(720);
    *config.mutable_window_name() = "Neko Editor";
    neko::editor::Editor editor;
    engine.RegisterSystem(editor);
    engine.RegisterOnEvent(editor);
    engine.RegisterOnDrawUi(editor);


    engine.SetWindowAndRenderer(&window, &renderer);
    engine.Init();
    engine.EngineLoop();
    return 0;
}
