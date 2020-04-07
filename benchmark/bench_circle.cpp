#include <benchmark/benchmark.h>
#include <mathematics/circle.h>
#include <random>
#include <iostream>
#include <mathematics/vector.h>

#ifdef EASY_PROFILE_USE
#include <easy/profiler.h>
#endif

const long fromRange = 8;
const long toRange = 1 << 20;
const float maxNmb = 100.0f;


float RandomFloat()
{
    static std::random_device rd;
    static std::mt19937 g(rd());
    static std::uniform_real_distribution<float> dist{ -100.0f, 100.0f };

    return dist(g);
}

static void BM_CircleIntersectsIntrasect(benchmark::State& state)
{
#ifdef EASY_PROFILE_USE
    EASY_PROFILER_ENABLE;
    EASY_BLOCK("TestCircleIntrasect", profiler::colors::Blue);
#endif
	
    const size_t n = state.range(0) / 4;
    std::vector<neko::FourCircle> vec;
    std::vector<neko::FourCircle> vec2;
	 
    std::array<neko::Circle, 4> array;
    std::array<neko::Circle, 4> array2;

    vec.reserve(n);
    vec2.reserve(n);
    /*array = { neko::Circle(neko::Vec2f(2.0f, 3.0f), 4.0f),
              neko::Circle(neko::Vec2f(4.0f, 5.0f), 4.0f),
              neko::Circle(neko::Vec2f(6.0f, 7.0f), 4.0f),
              neko::Circle(neko::Vec2f(11.0f, 13.0f), 4.0f) };

    array2 = { neko::Circle(neko::Vec2f(5.0f, 6.0f), 4.0f),
               neko::Circle(neko::Vec2f(7.0f, 8.0f), 4.0f),
               neko::Circle(neko::Vec2f(9.0f, 10.0f), 4.0f),
               neko::Circle(neko::Vec2f(9.0f, 10.0f), 4.0f) };*/
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("InitCircleIntrasect");
#endif
    for (size_t i = 0; i < n; i++)
    {
        for (size_t i = 0; i < 4; i++)
        {
            array[i] = neko::Circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat());
        }
    	for (size_t i = 0; i < 4; i++)
        {
            array2[i] = neko::Circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat());
        }
        

        neko::FourCircle n_circle(array);
        neko::FourCircle n_circle2(array2);

        vec.push_back(n_circle);
        vec2.push_back(n_circle2);
    }
    for (auto _ : state)
    {
        for (size_t i = 0; i < n ; i++)
        {
            //benchmark::DoNotOptimize(vec[i].IntersectsIntrinsics(vec2[i]));
            benchmark::DoNotOptimize(vec[i].IntersectsCircleIntrinsics(array2[1]));
        }

        
    }
	//auto end = std::chrono::steady_clock::now();
	//auto diff = end - start;
    //std::cout << std::chrono::duration <double, std::milli>(diff).count() << " ms\n";
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
    profiler::dumpBlocksToFile("circle_intrasect_profile.prof");
#endif
}
BENCHMARK(BM_CircleIntersectsIntrasect)->Range(fromRange, toRange);

static void BM_CircleIntersects(benchmark::State& state)
{
    const size_t n = state.range(0);
    std::vector<neko::Circle> v1;
	std::vector<neko::Circle> v2;
    v1.reserve(n);
	v2.reserve(n);
    auto start = std::chrono::steady_clock::now();

    for (size_t i = 0; i < n; i++)
    {
        v1.push_back( neko::Circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat()));
        v2.push_back( neko::Circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat()));
    }
	
    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(v1[i].Intersects(v2[i]));
        }
    }
    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << std::chrono::duration <double, std::milli>(diff).count() << " ms\n";
}
BENCHMARK(BM_CircleIntersects)->Range(fromRange, toRange);

static void BM_SphereIntersects(benchmark::State& state)
{
    const size_t n = state.range(0);
    std::vector<neko::Sphere> v1;
    std::vector<neko::Sphere> v2;
    v1.reserve(n);
    v2.reserve(n);

    for (size_t i = 0; i < n; i++)
    {
        v1.push_back(neko::Sphere(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat()));
        v2.push_back(neko::Sphere(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat()));
    }
	
    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(v1[i].Intersects(v2[i]));
        }
    }
}
BENCHMARK(BM_SphereIntersects)->Range(fromRange, toRange);

/*
static void BM_RectCircleIntersects(benchmark::State& state)
{
    const size_t n = state.range(0);
    std::vector<neko::Circle> v1;
    std::vector<neko::Rect2f> v2;
    v1.reserve(n);
    v2.reserve(n);
	
    for (size_t i = 0; i < n; i++)
    {
        v1.push_back(neko::Circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat()));
        v2.push_back(neko::Rect2f(neko::Vec2f(RandomFloat(), RandomFloat()), neko::Vec2f(RandomFloat(), RandomFloat())));
    }
	
    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(v1[i].RectCircleIntersects(v2[i]));
        	
        }
    }
}
BENCHMARK(BM_RectCircleIntersects)->Range(fromRange, toRange);


static void BM_PlanCircleIntersects(benchmark::State& state)
{
    const size_t n = state.range(0);
    std::vector<neko::Sphere> v1;
    std::vector<neko::Plan> v2;
    v1.reserve(n);
    v2.reserve(n);

    for (size_t i = 0; i < n; i++)
    {
        v1.push_back(neko::Sphere(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat()));
        v2.push_back(neko::Plan(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), neko::Vec3f(RandomFloat(),RandomFloat(), RandomFloat()).Normalized()));
    }
	
    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(v1[i].IsPlanSphereIntersects(v2[i]));
        }
    }
}
BENCHMARK(BM_PlanCircleIntersects)->Range(fromRange, toRange);
*/
