#include "asteroid/client.h"

#include "engine/conversion.h"

namespace neko::net
{
void Client::ReceivePacket(const asteroid::Packet* packet)
{
    const auto packetType = packet->packetType;
    switch (packetType)
    {
    case asteroid::PacketType::SPAWN_PLAYER:
    {
        const auto* spawnPlayerPacket = static_cast<const asteroid::SpawnPlayerPacket*>(packet);
        const auto clientId = ConvertFromBinary<ClientId>(spawnPlayerPacket->clientId);

        const PlayerNumber playerNumber = spawnPlayerPacket->playerNumber;
        if (clientId == clientId_)
        {
            gameManager_.SetClientPlayer(playerNumber);
        }

        const auto pos = ConvertFromBinary<Vec2f>(spawnPlayerPacket->pos);
        const auto rotation = ConvertFromBinary<degree_t>(spawnPlayerPacket->angle);

        gameManager_.SpawnPlayer(playerNumber, pos, rotation);
        break;
    }
    case asteroid::PacketType::START_GAME:
    {
        const auto* startGamePacket = static_cast<const asteroid::StartGamePacket*>(packet);
        unsigned long startingTime = ConvertFromBinary<unsigned long>(startGamePacket->startTime);
        gameManager_.StartGame(startingTime);
        break;
    }
    case asteroid::PacketType::INPUT:
    {
        const auto* playerInputPacket = static_cast<const asteroid::PlayerInputPacket*>(packet);
        const auto playerNumber = playerInputPacket->playerNumber;
        const auto inputFrame = ConvertFromBinary<Frame>(playerInputPacket->currentFrame);

        if (playerNumber == gameManager_.GetPlayerNumber())
        {
            //Verify the inputs coming back from the server
            const auto& inputs = gameManager_.GetRollbackManager().GetInputs(playerNumber);
            const auto currentFrame = gameManager_.GetRollbackManager().GetCurrentFrame();
            for (size_t i = 0; i < playerInputPacket->inputs.size(); i++)
            {
                const auto index = currentFrame - inputFrame + i;
                if (index > inputs.size())
                {
                    break;
                }
                if (inputs[index] != playerInputPacket->inputs[i])
                {
                    neko_assert(false, "Inputs coming back from server are not coherent!!!");
                }
                if (inputFrame - i == 0)
                {
                    break;
                }
            }
            break;
        }

        //discard delayed input packet
        if (inputFrame < gameManager_.GetRollbackManager().GetLastReceivedFrame(playerNumber))
        {
            break;
        }
        for (Frame i = 0; i < playerInputPacket->inputs.size(); i++)
        {
            gameManager_.SetPlayerInput(playerNumber,
                playerInputPacket->inputs[i],
                inputFrame - i);

            if (inputFrame - i == 0)
            {
                break;
            }
        }
        break;
    }
    case asteroid::PacketType::VALIDATE_STATE:
    {
        const auto* validateFramePacket = static_cast<const asteroid::ValidateFramePacket*>(packet);
        const auto newValidateFrame = ConvertFromBinary<Frame>(validateFramePacket->newValidateFrame);
        std::array<asteroid::PhysicsState, asteroid::maxPlayerNmb> physicsStates{};
        for (size_t i = 0; i < validateFramePacket->physicsState.size(); i++)
        {
            auto* statePtr = reinterpret_cast<std::uint8_t*>(physicsStates.data());
            statePtr[i] = validateFramePacket->physicsState[i];
        }
        gameManager_.ConfirmValidateFrame(newValidateFrame, physicsStates);
        //logDebug("Client received validate frame " + std::to_string(newValidateFrame));
        break;
    }
    case asteroid::PacketType::WIN_GAME:
    {
        const auto* winGamePacket = static_cast<const asteroid::WinGamePacket*>(packet);
        gameManager_.WinGame(winGamePacket->winner);
        break;
    }
    case asteroid::PacketType::SPAWN_BULLET: break;
    default:;
    }

}
}
