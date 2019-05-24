#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <condition_variable>
#include <queue>

namespace neko
{
class CommandBuffer
{
    virtual void Draw(sf::RenderTarget * window) = 0;
};

class GraphicsManager
{
public:
    void RenderLoop(std::condition_variable& condSyncRender, std::mutex& renderMutex);
private:
    std::queue<CommandBuffer*> commandBuffers;
};
}