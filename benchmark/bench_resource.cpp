#include <iostream>
#include <random>
#include "random_fill.h"
#include <benchmark/benchmark.h>


#include "engine/assert.h"
#include "engine/engine.h"
#include "engine/resource.h"
#include "utilities/file_utility.h"

const int fromRange = 4;
const int toRange = 512;


static void BM_SmallResourceThread(benchmark::State& state)
{
    neko::Configuration config;
    std::string path = config.dataRootPath + "data/test/smallFile.txt";
    neko::BufferFile fileContent;
    fileContent.Load(path);
    neko::ResourceManager resourceManager;
    resourceManager.Init();
    const size_t size = state.range(0);
    std::vector<neko::ResourceId> resourcesId;
    std::map<neko::ResourceId, neko::BufferFile> resources;
    for (int i = 0; i < size; i++)
    {
        resourcesId.push_back(resourceManager.LoadResource(path));
    }
    for (auto _ : state)
    {
        resources.clear();
        while (resources.size()<resourcesId.size()) {
            for (int n = 0; n < size; n++)
            {
                if (resources.find(resourcesId[n]) != resources.end())
                {
                    continue;
                }
                if (resourceManager.IsResourceReady(resourcesId[n]))
                {
                    resources[resourcesId[n]] = resourceManager.GetResource(resourcesId[n]);
                    neko_assert(fileContent == resources[resourcesId[n]], "Error Loading");
                }
            }
        }
    }
    resourceManager.Destroy();
}

static void BM_BigResourceThread(benchmark::State& state)
{
    neko::Configuration config;
    std::string path = config.dataRootPath + "data/test/bigFile.json";
    neko::BufferFile fileContent;
    fileContent.Load(path);
    neko::ResourceManager resourceManager;
    resourceManager.Init();
    const size_t size = state.range(0);
    std::vector<neko::ResourceId> resourcesId;
    std::map<neko::ResourceId, neko::BufferFile> resources;
    for (int i = 0; i < size; i++)
    {
        resourcesId.push_back(resourceManager.LoadResource(path));
    }
    for (auto _ : state)
    {
        resources.clear();
        while (resources.size() < resourcesId.size()) {
            for (int n = 0; n < size; n++)
            {
                if (resources.find(resourcesId[n]) != resources.end())
                {
                    continue;
                }
                if (resourceManager.IsResourceReady(resourcesId[n]))
                {
                    resources[resourcesId[n]] = resourceManager.GetResource(resourcesId[n]);
                    neko_assert(fileContent == resources[resourcesId[n]], "Error Loading");
                }
            }
        }
    }
    resourceManager.Destroy();
}

static void BM_SmallResourceNonThread(benchmark::State& state)
{
    neko::Configuration config;
    std::string path = config.dataRootPath + "data/test/smallFile.txt";
    neko::BufferFile fileContent;
    fileContent.Load(path);
    const size_t size = state.range(0);
    std::vector<neko::BufferFile> resources;
    for (auto _ : state)
    {
        resources.clear();
        for (int i = 0; i < size; i++)
        {
            neko::BufferFile loadedFile;
            loadedFile.Load(path);
            resources.push_back(loadedFile);
            neko_assert(fileContent == resources.back(), "Error Loading");
        }
    }
}

static void BM_BigResourceNonThread(benchmark::State& state)
{
    neko::Configuration config;
    std::string path = config.dataRootPath + "data/test/bigFile.json";
    neko::BufferFile fileContent;
    fileContent.Load(path);
    const size_t size = state.range(0);
    std::vector<neko::BufferFile> resources;
    for (auto _ : state)
    {
        resources.clear();
        for (int i = 0; i < size; i++)
        {
            neko::BufferFile loadedFile;
            loadedFile.Load(path);
            resources.push_back(loadedFile);
            neko_assert(fileContent == resources.back(), "Error Loading");
        }
    }
}

BENCHMARK(BM_SmallResourceThread)->Range(fromRange, toRange)->UseRealTime();
BENCHMARK(BM_BigResourceThread)->Range(fromRange, toRange)->UseRealTime();

BENCHMARK(BM_SmallResourceNonThread)->Range(fromRange, toRange)->UseRealTime();
BENCHMARK(BM_BigResourceNonThread)->Range(fromRange, toRange)->UseRealTime();