#include <benchmark/benchmark.h>
#include <random>
#include <algorithm>

const long fromRange = 8;
const long toRange = 1<<20;
const unsigned maxNmb = 1<<20;

bool is_prime_naive(unsigned int n)
{
    if (n == 1)
        return 0;
    if (n % 2 == 0)
        return false;
    for (unsigned int i = 3; i * i <= n; i += 2)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}

static std::vector<unsigned int> prime_numbers;
bool is_prime_dynamic(unsigned int n);
void generate_prime_up(unsigned n)
{
    auto biggest_prime = prime_numbers.back();
    for(auto i = biggest_prime+2; i * i <= n; i+=2)
    {
        if(is_prime_dynamic(i))
            prime_numbers.push_back(i);
    }
}

bool is_prime_dynamic(unsigned int n)
{
    if (n == 1)
        return false;

    for (auto p : prime_numbers)
    {
        if (__builtin_expect(p * p <= n, 1))
        {
            if (n % p == 0)
                return false;
        }
        else
        {
            return true;
        }
    }
    return true;
}

void RandomFill(std::vector<unsigned>& numbers, unsigned start = -maxNmb, unsigned end = maxNmb)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<unsigned> dist{start, end};
    std::generate(numbers.begin(), numbers.end(), [&g, &dist](){return dist(g);});
}

std::vector<unsigned int> local_numbers(toRange);


static void BM_NaivePrimeNumber(benchmark::State& state)
{
    for (auto _ : state)
    {
        for(size_t i = 0; i < state.range(0);i++)
        {
            benchmark::DoNotOptimize(is_prime_naive(local_numbers[i]));
        }
    }
}

BENCHMARK(BM_NaivePrimeNumber)->Range(fromRange, toRange);

static void BM_DynamicPrimeNumber(benchmark::State& state)
{
    for (auto _ : state)
    {
        for(size_t i = 0; i < state.range(0);i++)
        {
            benchmark::DoNotOptimize(is_prime_dynamic(local_numbers[i]));
        }
    }
}

BENCHMARK(BM_DynamicPrimeNumber)->Range(fromRange, toRange);

int main(int argc, char** argv)
{
    RandomFill(local_numbers, 2, maxNmb);
    prime_numbers = {2, 3, 5, 7, 11};
    generate_prime_up(maxNmb);

    for(auto n : local_numbers)
    {
        assert(is_prime_naive(n) == is_prime_dynamic(n));
    }

    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}