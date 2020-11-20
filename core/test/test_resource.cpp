#include <gtest/gtest.h>

#include "engine/resource.h"
#include "engine/engine.h"

class Engine : public neko::BasicEngine
{
public:
    using neko::BasicEngine::BasicEngine;
    void ManageEvent() override{}
    void Stop()
    {
        isRunning_ = false;
    }
};

class ResourceResultSystem : public neko::SystemInterface
{
public:
    explicit ResourceResultSystem(
        neko::ResourceManager& resourceManager,
        Engine& engine);
    void Init() override;
    void Update(neko::seconds dt) override;
    void Destroy() override;
    void AddFileToLoad(std::string_view path)
    {
        paths_.push_back(path.data());
    }
private:
    Engine& engine_;
    neko::ResourceManager& resourceManager_;
    std::vector<std::string> paths_;
    std::vector<neko::ResourceId> resourceIds_;
};

ResourceResultSystem::ResourceResultSystem(
    neko::ResourceManager& resourceManager, Engine& engine):
    engine_(engine), resourceManager_(resourceManager)
{
}

void ResourceResultSystem::Init()
{
    for(auto& path : paths_)
    {
        auto resourceId = resourceManager_.LoadResource(path);
        resourceIds_.push_back(resourceId);
    }
}

void ResourceResultSystem::Update(neko::seconds dt)
{
    
    const auto* resource = resourceManager_.GetResource(resourceIds_.back());
    if(resource != nullptr)
    {
        for(auto resourceId : resourceIds_)
        {
            resourceManager_.RemoveResource(resourceId);
        }
        engine_.Stop();
    }
}

void ResourceResultSystem::Destroy()
{
}

TEST(Engine, TestOneResource)
{
    Engine engine{nullptr};
    neko::ResourceManager resourceManager;
    ResourceResultSystem result{ resourceManager, engine };
    result.AddFileToLoad("../data/sprites/wall.jpg");
    engine.RegisterSystem(resourceManager);
    engine.RegisterSystem(result);
    engine.Init();
    engine.EngineLoop();

}

TEST(Engine, TestSeveralResource)
{
    Engine engine{ nullptr };
    neko::ResourceManager resourceManager;
    ResourceResultSystem result{ resourceManager, engine };
    result.AddFileToLoad("../data/sprites/wall.jpg");
    //result.AddFileToLoad("../data/sprites/wall.jpg.ktx");
    result.AddFileToLoad("../data/sprites/grass.png");
    engine.RegisterSystem(resourceManager);
    engine.RegisterSystem(result);
    engine.Init();
    engine.EngineLoop();

}