#include <benchmark/benchmark.h>
#include <random>
#include <algorithm>
#include <numeric>
#include <vector>
#include "engine/array.h"
#include <mathematics/vector.h>

const long fromRange = 8;
const long toRange = 1 << 15;
const float start = -100.0f;
const float end = 100.0f;



BENCHMARK_MAIN();