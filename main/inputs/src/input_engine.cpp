#include "input/input_engine.h"

namespace neko
{
InputEngine::InputEngine(Configuration* config) : SdlEngine(config)
{
    RegisterSystem(sampleBrowser_);
    RegisterOnDrawUi(sampleBrowser_);
    RegisterOnEvent(sampleBrowser_);
}

void InputEngine::Destroy()
{
    sampleBrowser_.Destroy();
    SdlEngine::Destroy();
}
}
