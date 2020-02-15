#pragma once

#ifndef EMSCRIPTEN
#ifdef WIN32
#include <intrin.h>
#ifdef __AVX2__
#define __SSE__
#define __SSE4_2__
#define __FMA__
#endif
#else
#include <x86intrin.h>
#endif
#else
#include <wasm_simd128.h>
typedef float v4f __attribute__ ((vector_size (16)));
#endif