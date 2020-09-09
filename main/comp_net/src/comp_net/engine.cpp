#include "comp_net/engine.h"

namespace neko::net
{
CompNetEngine::CompNetEngine(Configuration* config) : SdlEngine(config)
{
    RegisterSystem(sampleBrowser_);
    RegisterOnDrawUi(sampleBrowser_);
    RegisterOnEvent(sampleBrowser_);
}

void CompNetEngine::Destroy()
{
    sampleBrowser_.Destroy();
    SdlEngine::Destroy();
}
}
