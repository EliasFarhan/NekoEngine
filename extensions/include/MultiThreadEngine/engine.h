#pragma once

#include <engine/engine.h>

namespace multi
{
class GraphicsManager;
/**
 * \brief main loop engine class, it runs its own graphics manager thread and manage the window events
 */
class MainEngine : public neko::BasicEngine
{
public:
    explicit MainEngine(neko::Configuration* config = nullptr);

    virtual ~MainEngine();

    void Init() override;

    void Update(float dt) override;

    void Destroy() override;
    /**
     * \brief completely override the BasicEngine loop due to the render thread synchronization
     */
    void EngineLoop() override;

    virtual void OnBeginContact(const neko::Collider* colliderA, const neko::Collider* colliderB)
    {}

    virtual void OnEndContact(const neko::Collider* colliderA, const neko::Collider* colliderB)
    {}

    GraphicsManager* GetGraphicsManager() const;



    sf::Vector2u renderTargetSize;

    //used to sync with the render thread
    std::condition_variable condSyncRender;
    std::mutex renderStartMutex;

    std::atomic<neko::Index> frameIndex = 0u;
protected:
    ctpl::thread_pool workingThreadPool_;
    std::thread renderThread_;
    std::unique_ptr<GraphicsManager> graphicsManager_ = nullptr;
};

}