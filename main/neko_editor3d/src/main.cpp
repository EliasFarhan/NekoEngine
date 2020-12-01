#include "editor_filesystem.h"
#include "model_viewer.h"

#include <editor_engine.h>
#include "gl/graphics.h"
#include "gl/gles3_window.h"


int main(int argc, char** argv)
{
  neko::sdl::Gles3Window window;
  neko::gl::Gles3Renderer renderer;
  neko::editor::EditorFilesystem editorFilesystem;
  neko::editor::EditorEngine engine{editorFilesystem};

  neko::editor::ModelViewer modelViewer;

  engine.RegisterSystem(modelViewer);
  engine.RegisterOnDrawUi(modelViewer);

  engine.SetWindowAndRenderer(&window, &renderer);
  engine.Init();
  engine.EngineLoop();
  return 0;
}
