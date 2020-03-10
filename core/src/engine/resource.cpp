#include <engine/resource.h>
#include "utilities/file_utility.h"

neko::ResourceManager::ResourceManager() {
    loadingThread_ = std::thread([this]() {
        isRunning_ = true;
        this->LoadingLoop();
        });
}

void neko::ResourceManager::Close() {
    isRunning_ = false;
    cv_.notify_one();
    loadingThread_.join();
}

void neko::ResourceManager::LoadingLoop() {
    std::unique_lock<std::mutex> lock(loadingMutex_);
    while (isRunning_) {
       if (!idQueue_.empty()) {
           resourceLoaded_[idQueue_[0]] = LoadFile(resourcePath_[idQueue_[0]].data());
           resourcePath_.erase(idQueue_[0]);
           idQueue_.erase(idQueue_.begin());
       } else {
           cv_.wait(lock);
       }
    }
}

bool neko::ResourceManager::IsResourceReady(ResourceId resourceId) {
    const std::lock_guard<std::mutex> lockGuard(loadingMutex_);
    return resourceLoaded_.find(resourceId) != resourceLoaded_.end();
}


std::string neko::ResourceManager::GetResource(ResourceId resourceId) {
    const std::lock_guard<std::mutex> lockGuard(loadingMutex_);
    if (resourceLoaded_.find(resourceId) != resourceLoaded_.end()) {
        return resourceLoaded_[resourceId];
    }
    return "";
}

neko::ResourceId neko::ResourceManager::LoadResource(const Path assetPath)
{
    const std::lock_guard<std::mutex> lockGuard(loadingMutex_);
    const ResourceId resourceId = sole::uuid4();
    idQueue_.push_back(resourceId);
    resourcePath_[resourceId] = assetPath;
    cv_.notify_all();
    return resourceId;
}
