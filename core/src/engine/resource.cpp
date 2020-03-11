#include <engine/resource.h>
#include "utilities/file_utility.h"
#include "utilities/json_utility.h"

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
           //resourceFutures_[idQueue_[0]].set_value(LoadFile(resourcePath_[idQueue_[0]].data()).c_str());
           const size_t extensionIndex = resourcePath_[idQueue_[0]].find_last_of('.');
           std::string metaFilePath = resourcePath_[idQueue_[0]].data();
           metaFilePath = metaFilePath.substr(0, extensionIndex).append(".meta");
           json metaJson = json::object();
           metaJson["resourceID"] = json::string_t();
           metaJson["resourceID"] = idQueue_[0].str();
           WriteStringToFile(metaFilePath, metaJson.dump(4));
           resourcePath_.erase(idQueue_[0]);
           idQueue_.erase(idQueue_.begin());
       } else {
           cv_.wait(lock);
       }
    }
}

bool neko::ResourceManager::IsResourceReady(ResourceId resourceId) {
    const std::lock_guard<std::mutex> lockGuard(loadingMutex_);
    //return resourceFutures_[resourceId].get_future()._Is_ready();
    return resourceLoaded_.find(resourceId) != resourceLoaded_.end();
}


std::string neko::ResourceManager::GetResource(ResourceId resourceId) {
    const std::lock_guard<std::mutex> lockGuard(loadingMutex_);
    if (resourceLoaded_.find(resourceId) != resourceLoaded_.end()) {
        return resourceLoaded_[resourceId];
    }
    /*if (resourceFutures_.find(resourceId) != resourceFutures_.end()) {
        const char* value = resourceFutures_[resourceId].get_future().get();
        resourceFutures_.erase(resourceId);
        return value;
    }*/
    return "";
}

neko::ResourceId neko::ResourceManager::LoadResource(const Path assetPath)
{
    const std::lock_guard<std::mutex> lockGuard(loadingMutex_);
    const ResourceId resourceId = sole::uuid4();
    idQueue_.push_back(resourceId);
    resourcePath_[resourceId] = assetPath;
    //std::promise<const char*> promise;
    //std::future<const char*> future = promise.get_future();
    //resourceFutures_[resourceId] = std::move(promise);
    cv_.notify_all();
    return resourceId;
}
