#include <vector>
#include <algorithm>
#include <random>
#include <benchmark/benchmark.h>

const unsigned long fromRange = 1<<10;
const unsigned long toRange = 1<<20;

const int maxNmb = 100'000;

int is_even(int a)
{
    return a % 2 == 0;
}

size_t count_even_raw(const std::vector<int>& array)
{
    size_t count = 0;
    const size_t size = array.size();
    for(size_t i = 0; i < array.size();i++)
    {
        if(is_even(array[i]))
            ++count;
    }
    return count;
}

// Type your code here, or load an example.
size_t count_even_ranges(const std::vector<int>& array)
{
    size_t count = 0;
    for(const auto& a : array)
    {
        if(is_even(a))
            ++count;
    }
    return count;
}

// Type your code here, or load an example.
size_t count_even_iterator(const std::vector<int>& array)
{
    size_t count = 0;
    for(auto it = array.cbegin();it != array.cend();++it)
    {
        if(is_even(*it))
            ++count;
    }
    return count;
}

size_t count_even_algo(const std::vector<int>& array)
{
    return std::count_if(array.cbegin(), array.cend(), is_even);
}

static void RandomFill(std::vector<int>& numbers)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<int> dist{1, maxNmb};
    std::generate(numbers.begin(), numbers.end(), [&g, &dist](){return dist(g);});
}
static void BM_Raw(benchmark::State& state)
{
    std::vector<int> local_numbers(state.range(0));
    RandomFill(local_numbers);
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(count_even_raw(local_numbers));
    }
}
BENCHMARK(BM_Raw)->Range(fromRange, toRange);

static void BM_Range(benchmark::State& state)
{
    std::vector<int> local_numbers(state.range(0));
    RandomFill(local_numbers);
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(count_even_ranges(local_numbers));
    }
}
BENCHMARK(BM_Range)->Range(fromRange, toRange);

static void BM_Iterator(benchmark::State& state)
{
    std::vector<int> local_numbers(state.range(0));
    RandomFill(local_numbers);
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(count_even_iterator(local_numbers));
    }
}
BENCHMARK(BM_Iterator)->Range(fromRange, toRange);
static void BM_Algo(benchmark::State& state)
{
    std::vector<int> local_numbers(state.range(0));
    RandomFill(local_numbers);
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(count_even_algo(local_numbers));
    }
}
BENCHMARK(BM_Algo)->Range(fromRange, toRange);

int main(int argc, char** argv)
{
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}