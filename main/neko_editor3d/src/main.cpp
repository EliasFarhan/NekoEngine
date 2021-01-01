#include "editor_filesystem.h"
#include "model_viewer.h"

#include <editor_engine.h>
#include "gl/graphics.h"
#include "gl/gles3_window.h"


int main([[nodiscard]] int argc, [[nodiscard]]char** argv)
{
  neko::sdl::Gles3Window window;
  neko::gl::Gles3Renderer renderer;
  neko::editor::EditorFilesystem editorFilesystem;
  neko::sdl::SdlEngine engine(editorFilesystem);
  neko::editor::Editor editor;
  engine.RegisterSystem(editor);
  engine.RegisterOnEvent(editor);
  engine.RegisterOnDrawUi(editor);


  engine.SetWindowAndRenderer(&window, &renderer);
  engine.Init();
  engine.EngineLoop();
  return 0;
}
