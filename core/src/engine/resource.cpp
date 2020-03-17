#include <engine/resource.h>
#include "utilities/file_utility.h"
#include "utilities/json_utility.h"

neko::ResourceManager::ResourceManager() {
    status_ |= IS_RUNNING;
    loadingThread_ = std::thread([this]() {
        this->LoadingLoop();
        });
}

void neko::ResourceManager::Destroy() {
    status_ &= ~IS_RUNNING;
    cv_.notify_one();
    loadingThread_.join();
}

void neko::ResourceManager::LoadingLoop() {
    while (status_ & IS_RUNNING) {
       if (status_ & IS_NOT_EMPTY) {
           std::lock_guard<std::mutex> lockGuard(loadingMutex_);
           Path path = resourcePromises_[idQueue_[0]].path;
           resourcePromises_[idQueue_[0]].resource = LoadFile(path);
           resourcePromises_[idQueue_[0]].ready = true;
           const size_t extensionIndex = path.find_last_of('.');
           std::string metaFilePath = path.data();
           metaFilePath = metaFilePath.substr(0, extensionIndex).append(".meta");
           json metaJson = json::object();
           metaJson["resourceID"] = json::string_t();
           metaJson["resourceID"] = idQueue_[0].str();
           WriteStringToFile(metaFilePath, metaJson.dump(4));
           idQueue_.erase(idQueue_.begin());
           if (idQueue_.empty())
           {
               status_ &= ~IS_NOT_EMPTY;
           }
       } else {
           std::unique_lock<std::mutex> lock(loadingMutex_);
           if (!(status_ & IS_NOT_EMPTY)) {
               cv_.wait(lock);
           }
       }
    }
}

bool neko::ResourceManager::IsResourceReady(ResourceId resourceId) {
    const std::lock_guard<std::mutex> lockGuard(loadingMutex_);
    return resourcePromises_[resourceId].ready;
}


std::string neko::ResourceManager::GetResource(ResourceId resourceId) {
    const std::lock_guard<std::mutex> lockGuard(loadingMutex_);
    return resourcePromises_[resourceId].resource;
}

neko::ResourceId neko::ResourceManager::LoadResource(const Path assetPath)
{
    const std::lock_guard<std::mutex> lockGuard(loadingMutex_);
    const ResourceId resourceId = sole::uuid4();
    idQueue_.push_back(resourceId);
    resourcePromises_[resourceId] = LoadPromise(assetPath);
    status_ |= IS_NOT_EMPTY;
    cv_.notify_all();
    return resourceId;
}
