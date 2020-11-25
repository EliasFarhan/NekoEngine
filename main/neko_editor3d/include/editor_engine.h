#pragma once

#include <sdl_engine/sdl_engine.h>

namespace neko::editor
{
class EditorEngine : public sdl::SdlEngine
{
public:
	using SdlEngine::SdlEngine;
};
}