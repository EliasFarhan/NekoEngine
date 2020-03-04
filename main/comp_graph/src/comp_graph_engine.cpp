#include "comp_graph/comp_graph_engine.h"

namespace neko
{
CompGraphEngine::CompGraphEngine(Configuration* config) : SdlEngine(config)
{
    RegisterSystem(sampleBrowser_);
    RegisterOnDrawUi(sampleBrowser_);
    RegisterOnEvent(sampleBrowser_);
}

void CompGraphEngine::Destroy()
{
    sampleBrowser_.Destroy();
    SdlEngine::Destroy();
}
}
