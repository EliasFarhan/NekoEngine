#include <benchmark/benchmark.h>
#include <engine/entity.h>

#include <chrono>
#include <thread>

using namespace std::chrono_literals;
using namespace neko;
const unsigned long fromRange = 8;
const unsigned long toRange = 32768;

const ComponentType componentType = 1;
const ComponentType otherComponentType = 2;

void DoSomethingWithBranch(EntityManager& entityManager)
{
	const auto length = entityManager.GetEntitiesSize();
	for (Entity i = 0; i < length; i++)
	{
		if (entityManager.HasComponent(i, componentType))
		{
			std::this_thread::sleep_for(100ns);
		}
	}
}

void DoSomethingWithFilter(EntityManager& entityManager)
{
	std::vector<Entity> entities;
	entities.reserve(entityManager.GetEntitiesSize());
	for (const auto& entity: entities)
	{
		if (entityManager.HasComponent(entity, componentType))
		{
			entities.push_back(entity);
		}
	}
	for (auto& entity : entities)
	{
		std::this_thread::sleep_for(100ns);
	}
}


static void BM_Foo(benchmark::State& state) 
{
	const unsigned long length = state.range(0);
	EntityManager entityManager;
	//Fill randomly entityManager
	for (unsigned long i = 0; i < state.range(0); i++)
	{
		const auto entity = entityManager.CreateEntity();
		entityManager.AddComponentType(entity, rand() % 2 == 0?componentType: otherComponentType);
	}
	for (auto _ : state) 
	{
		DoSomethingWithBranch(entityManager);
	}
}
BENCHMARK(BM_Foo)->Range(fromRange, toRange);


static void BM_FooAlias(benchmark::State& state) 
{
	EntityManager entityManager;
	//Fill randomly entityManager
	for (unsigned long i = 0; i < state.range(0); i++)
	{
		const auto entity = entityManager.CreateEntity();
		entityManager.AddComponentType(entity, rand() % 2 == 0 ? componentType : otherComponentType);
	}
	for (auto _ : state) 
	{
		DoSomethingWithFilter(entityManager);
		
	}
}
BENCHMARK(BM_FooAlias)->Range(fromRange, toRange);

BENCHMARK_MAIN();