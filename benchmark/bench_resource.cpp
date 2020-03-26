#include <iostream>
#include <random>
#include "random_fill.h"
#include <benchmark/benchmark.h>

#include "engine/resource.h"
#include "utilities/file_utility.h"

const int fromRange = 2;
const int toRange = 1024;


static void BM_ResourceThread(benchmark::State& state)
{
    std::string path = "./../../data/test/test.txt";
    std::string test = neko::LoadFile(path);
    neko::ResourceManager resourceManager;
    const size_t size = state.range(0);
    std::vector<neko::ResourceId> resourcesId;
    std::map<neko::ResourceId, std::string> resources;
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
                    if (test != resources[resourcesId[n]]) {
                        std::cout << "Error Loading" << '\n';
                    }
                }
            }
        }
    }
    resourceManager.Destroy();
}



static void BM_ResourceNonThread(benchmark::State& state)
{
    std::string path = "./../../data/test/test.txt";
    std::string test = neko::LoadFile(path);
    const size_t size = state.range(0);
    std::vector<std::string> resources;
    for (auto _ : state)
    {
        resources.clear();
        for (int i = 0; i < size; i++)
        {
            resources.push_back(neko::LoadFile(path));
            if (test != resources.back()) {
                std::cout << "Error Loading" << '\n';
            }
        }
    }
}
BENCHMARK(BM_ResourceThread)->Range(fromRange, toRange)->UseRealTime();

BENCHMARK(BM_ResourceNonThread)->Range(fromRange, toRange)->UseRealTime();