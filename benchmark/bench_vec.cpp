//
// Created by efarhan on 02.12.19.
//
#include <benchmark/benchmark.h>
#include <mathematics/vector.h>
#include <vector>
#include <random>
#include <iostream>

const long fromRange = 8;
const long toRange = 1<<15;
const float maxNmb = 100.0f;


void RandomFill(std::vector<neko::Vec3f>& numbers, float start = -maxNmb, float end = maxNmb)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{start, end};
    std::generate(numbers.begin(), numbers.end(), [&g, &dist](){return neko::Vec3f(dist(g), dist(g), dist(g));});
}
void RandomFill(std::vector<neko::Vec4f>& numbers, float start = -maxNmb, float end = maxNmb)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{start, end};
    std::generate(numbers.begin(), numbers.end(), [&g, &dist](){return neko::Vec4f(dist(g),dist(g),dist(g),0.0f);});
}

static void BM_Vec3Magnitude(benchmark::State& state)
{

    std::vector<neko::Vec3f> local_numbers(state.range(0));
    RandomFill(local_numbers, 0.0f, maxNmb);
    assert(reinterpret_cast<uintptr_t>(&local_numbers[0]) % alignof(__STDCPP_DEFAULT_NEW_ALIGNMENT__) == 0);
    for (auto _ : state)
    {
        for(auto v : local_numbers)
        {
            float f = v.GetMagnitude();
            benchmark::DoNotOptimize(f);
        }
    }
}
BENCHMARK(BM_Vec3Magnitude)->Range(fromRange, toRange);

static void BM_Vec4Magnitude(benchmark::State& state)
{
    std::vector<neko::Vec4f> local_numbers(state.range(0));
    RandomFill(local_numbers, 0.0f, maxNmb);
    assert(reinterpret_cast<uintptr_t>(&local_numbers[0]) % alignof(__STDCPP_DEFAULT_NEW_ALIGNMENT__) == 0);
    for (auto _ : state)
    {
        for(auto v : local_numbers)
        {
            float f = std::sqrt(neko::Vec4f::Dot3(v,v));
            benchmark::DoNotOptimize(f);
        }
    }

}
BENCHMARK(BM_Vec4Magnitude)->Range(fromRange, toRange);
static void BM_Vec3MagnitudeAoSoA(benchmark::State& state)
{
    std::vector<neko::Vec3f> local_numbers(state.range(0));
    RandomFill(local_numbers, 0.0f, maxNmb);
    assert(reinterpret_cast<uintptr_t>(&local_numbers[0]) % alignof(__STDCPP_DEFAULT_NEW_ALIGNMENT__) == 0);
    neko::FourVec4f tmp;
    for (auto _ : state)
    {
        for(size_t i = 0; i<local_numbers.size();i+=4)
        {
            tmp = neko::FourVec4f(&local_numbers[i]);
            benchmark::DoNotOptimize(tmp.Magnitude());
        }
    }

}
BENCHMARK(BM_Vec3MagnitudeAoSoA)->Range(fromRange, toRange);
static void BM_Vec4MagnitudeAoSoA(benchmark::State& state)
{
    std::vector<neko::Vec4f> local_numbers(state.range(0));
    RandomFill(local_numbers, 0.0f, maxNmb);
    assert(reinterpret_cast<uintptr_t>(&local_numbers[0]) % alignof(__STDCPP_DEFAULT_NEW_ALIGNMENT__) == 0);
    neko::FourVec4f tmp;
    for (auto _ : state)
    {
        for(size_t i = 0; i<local_numbers.size();i+=4)
        {
            tmp = neko::FourVec4f(&local_numbers[i]);
            benchmark::DoNotOptimize(tmp.Magnitude());
        }
    }

}
BENCHMARK(BM_Vec4MagnitudeAoSoA)->Range(fromRange, toRange);

int main(int argc, char** argv)
{
    /*
    std::cout << "__STDCPP_DEFAULT_NEW_ALIGNMENT__ is "
              << __STDCPP_DEFAULT_NEW_ALIGNMENT__ << std::endl;

    std::cout << "sizeof(Vec3f) is " << sizeof(neko::Vec3f) << '\n';
    std::cout << "alignof(Vec3f) is " << alignof(neko::Vec3f) << '\n';
    std::cout << "sizeof(Vec4f) is " << sizeof(neko::Vec4f) << '\n';
    std::cout << "alignof(Vec4f) is " << alignof(neko::Vec4f) << '\n';
    */
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}