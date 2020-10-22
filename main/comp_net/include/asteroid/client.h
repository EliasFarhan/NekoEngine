#pragma once
#include "game_manager.h"
#include "packet_type.h"
#include "graphics/graphics.h"

namespace neko::net
{
    class Client : public RenderProgram, public DrawImGuiInterface, public asteroid::PacketSenderInterface
    {
    public:
        Client() : gameManager_(*this)
        {
            
        }
        void SetWindowSize(Vec2u windowSize)
        {
            gameManager_.SetWindowSize(windowSize);
        }
    protected:

        asteroid::ClientGameManager gameManager_;
        ClientId clientId_ = 0;
    };
}
