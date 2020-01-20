#include <benchmark/benchmark.h>
#include <cstdlib>
#include <iostream>
#include <cmath>

const unsigned long fromRange = 8;
const unsigned long toRange = 1<<13;

class Matrix {
public:
    Matrix(size_t n) : n(n) {
        numbers.resize(n * n);
        for (size_t i = 0; i < n; i++) {
            numbers[i] = rand();
        }
    }

    const int& operator()(size_t x, size_t y) const {

        return numbers[x * n + y];
    }

    int& operator()(size_t x, size_t y) {

        return numbers[x * n + y];
    }

    const size_t size() const { return n; }

private:
    size_t n;
    std::vector<int> numbers;
};


static void BM_Row(benchmark::State& state) {
    const size_t n = state.range(0);
    Matrix m(n);
    for (auto _ : state) {
        for (size_t i = 0; i < m.size(); i++) {
            for (size_t j = 0; j < m.size(); j++) {
                m(i, j) += j;
            }
        }
    }

    state.counters["KB"] = n * n * sizeof(int) / 1024;
}

BENCHMARK(BM_Row)->RangeMultiplier(2)->Range(fromRange, toRange);


static void BM_Column(benchmark::State& state) {

    const size_t n = state.range(0);
    Matrix m(n);
    for (auto _ : state) {
        for (size_t i = 0; i < m.size(); i++) {
            for (size_t j = 0; j < m.size(); j++) {
                m(j, i) += j;
            }
        }
    }
    state.counters["KB"] = n * n * sizeof(int) / 1024;
}

BENCHMARK(BM_Column)->RangeMultiplier(2)->Range(fromRange, toRange);

static void BM_RowWithWork(benchmark::State& state) {
    const size_t n = state.range(0);
    Matrix m(n);
    for (auto _ : state) {
        for (size_t i = 0; i < m.size(); i++) {
            for (size_t j = 0; j < m.size(); j++) {
                m(i, j) += std::sqrt(std::hash<int>()(j*n+i));
            }
        }
    }

    state.counters["KB"] = n * n * sizeof(int) / 1024;
}

BENCHMARK(BM_RowWithWork)->RangeMultiplier(2)->Range(fromRange, toRange);


static void BM_ColumnWithWork(benchmark::State& state) {

    const size_t n = state.range(0);
    Matrix m(n);
    for (auto _ : state) {
        for (size_t i = 0; i < m.size(); i++) {
            for (size_t j = 0; j < m.size(); j++) {
                m(j, i) += std::sqrt(std::hash<int>()(i*n+j));
            }
        }
    }
    state.counters["KB"] = n * n * sizeof(int) / 1024;
}

BENCHMARK(BM_ColumnWithWork)->RangeMultiplier(2)->Range(fromRange, toRange);


static void BM_Random(benchmark::State& state) {
    const size_t n = state.range(0);
    Matrix m(n);
    for (auto _ : state) {
        for (size_t i = 0; i < m.size(); i++) {
            for (size_t j = 0; j < m.size(); j++) {
                m(j, rand() % n) += j;
            }
        }
    }

    state.counters["KB"] = n * n * sizeof(int) / 1024;
}

BENCHMARK(BM_Random)->RangeMultiplier(2)->Range(fromRange, toRange);

BENCHMARK_MAIN();