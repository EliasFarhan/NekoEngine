#include "comp_net/engine.h"

namespace neko::net
{
RdtEngine::RdtEngine(Configuration* config) : SdlEngine(config)
{
    RegisterSystem(sampleBrowser_);
    RegisterOnDrawUi(sampleBrowser_);
    RegisterOnEvent(sampleBrowser_);
}

void RdtEngine::Destroy()
{
    sampleBrowser_.Destroy();
    SdlEngine::Destroy();
}
}
