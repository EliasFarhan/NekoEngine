#include <engine/resource.h>

neko::ResourceManager::ResourceManager() {
    thread_ = std::thread([this]() {
        isRunning_ = true;
        this->LoadingLoop();
        });
}

void neko::ResourceManager::Close() {
    isRunning_ = false;
    cv_.notify_one();
    thread_.join();
}

void neko::ResourceManager::LoadingLoop() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (isRunning_) {
       if (!pathQueue_.empty()) {
           resourceLoaded_[pathQueue_.back()] = LoadFile(resourcePathQueue_[pathQueue_.back()]);
           resourcePathQueue_.erase(pathQueue_.back());
           pathQueue_.pop_back();
       } else {
           cv_.wait(lock);
       }
    }
}

bool neko::ResourceManager::IsResourceReady(ResourceId resourceId) {
    return resourceLoaded_.find(resourceId) != resourceLoaded_.end();
}


std::string neko::ResourceManager::GetResource(ResourceId resourceId) {
    if (resourceLoaded_.find(resourceId) != resourceLoaded_.end()) {
        return resourceLoaded_[resourceId];
    }
    return "";
}

neko::ResourceId neko::ResourceManager::LoadResource(const std::string assetPath)
{
    ResourceId resourceId = sole::uuid0();
    pathQueue_.push_back(resourceId);
    resourcePathQueue_[resourceId] = assetPath;
    cv_.notify_all();
    return resourceId;
}
