#pragma once

#if defined(EMSCRIPTEN)
#include <wasm_simd128.h>
typedef float v4f __attribute__ ((vector_size (16)));
#endif

#ifdef WIN32
#include <intrin.h>
#ifdef __AVX2__
#define __SSE__
#define __SSE4_2__
#define __FMA__
#else
static_assert(false, "AVX2 is not enabled");
#endif
#endif

#if defined(__arm__) || defined(__ANDROID__)
typedef float v4f __attribute__ ((vector_size (16)));
#endif

#if defined(__amd64__) || defined(__i386__)
#include <x86intrin.h>
#endif
