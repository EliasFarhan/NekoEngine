#include <iostream>
// #include <random>
// #include "random_fill.h"
#include <benchmark/benchmark.h>
#include <mathematics/transform.h>

using namespace neko;

const long from = 2;
const long to = 1024;

void Translate(Transform3d& mat, const Vec3f translation)
{
    mat.Translate(translation);
}

static void BM_Translate(benchmark::State& s)
{
    int arraySize = s.range(0);
    std::vector<Transform3d> transforms(arraySize);
    std::vector<Vec3f> translations(arraySize);

    for (int i = 0; i < arraySize; ++i)
    {
        translations[i] = Vec3f(rand(), rand(), rand());
        transforms[i] = Transform3d::Identity;
    }

    for (auto _ : s)
    {
        for (int i = 0; i < arraySize; ++i)
        {
            Translate(transforms[i], translations[i]);
        }
    }
}

BENCHMARK(BM_Translate)->Range(from, to);

void Scale(Transform3d& mat, const Vec3f scale)
{
    mat.Scale(scale);
}

static void BM_Scale(benchmark::State& s)
{
    int arraySize = s.range(0);
    std::vector<Transform3d> transforms(arraySize);
    std::vector<Vec3f> scales(arraySize);

    for (int i = 0; i < arraySize; ++i)
    {
        scales[i] = Vec3f(rand(), rand(), rand());
        transforms[i] = Transform3d::Identity;
    }

    for (auto _ : s)
    {
        for (int i = 0; i < arraySize; ++i)
        {
            Scale(transforms[i], scales[i]);
        }
    }
}

BENCHMARK(BM_Scale)->Range(from, to);

void RotateAroundAxisDegrees(Transform3d& mat, const degree_t angle, const Vec3f axis)
{
    mat.Rotate(angle, axis);
}

static void BM_RotateAroundAxisDegrees(benchmark::State& s)
{
    int arraySize = s.range(0);
    std::vector<Transform3d> transforms(arraySize);
    std::vector<Vec3f> axes(arraySize);
    std::vector<degree_t> angles(arraySize);

    for (int i = 0; i < arraySize; ++i)
    {
        axes[i] = Vec3f(rand(), rand(), rand()).Normalized();
        transforms[i] = Transform3d::Identity;
        angles[i] = degree_t(rand());
    }

    for (auto _ : s)
    {
        for (int i = 0; i < arraySize; ++i)
        {
            RotateAroundAxisDegrees(transforms[i], angles[i], axes[i]);
        }
    }
}

BENCHMARK(BM_RotateAroundAxisDegrees)->Range(from, to);

void RotateAroundAxisRadians(Transform3d& mat, const radian_t angle, const Vec3f axis)
{
    mat.Rotate(angle, axis);
}

static void BM_RotateAroundAxisRadians(benchmark::State& s)
{
    int arraySize = s.range(0);
    std::vector<Transform3d> transforms(arraySize);
    std::vector<Vec3f> axes(arraySize);
    std::vector<radian_t> angles(arraySize);

    for (int i = 0; i < arraySize; ++i)
    {
        axes[i] = Vec3f(rand(), rand(), rand()).Normalized();
        transforms[i] = Transform3d::Identity;
        angles[i] = radian_t(rand());
    }

    for (auto _ : s)
    {
        for (int i = 0; i < arraySize; ++i)
        {
            RotateAroundAxisRadians(transforms[i], angles[i], axes[i]);
        }
    }
}

BENCHMARK(BM_RotateAroundAxisRadians)->Range(from, to);

void RotateUsingCardinals(Transform3d& mat, const EulerAngles angles)
{
    mat.Rotate(angles);
}

static void BM_RotateUsingCardinals(benchmark::State& s)
{
    int arraySize = s.range(0);
    std::vector<Transform3d> transforms(arraySize);
    std::vector<EulerAngles> angles(arraySize);

    for (int i = 0; i < arraySize; ++i)
    {
        angles[i] = EulerAngles(degree_t(rand()), degree_t(rand()), degree_t(rand()));
        transforms[i] = Transform3d::Identity;
    }

    for (auto _ : s)
    {
        for (int i = 0; i < arraySize; ++i)
        {
            RotateUsingCardinals(transforms[i], angles[i]);
        }
    }
}

BENCHMARK(BM_RotateUsingCardinals)->Range(from, to);

void RotateUsingQuaternion(Transform3d& mat, const Quaternion q)
{
    mat.Rotate(q);
}

static void BM_RotateUsingQuaternion(benchmark::State& s)
{
    int arraySize = s.range(0);
    std::vector<Transform3d> transforms(arraySize);
    std::vector<Quaternion> quaternions(arraySize);

    for (int i = 0; i < arraySize; ++i)
    {
        quaternions[i] = Quaternion(rand(), rand(), rand(), rand());
        transforms[i] = Transform3d::Identity;
    }

    for (auto _ : s)
    {
        for (int i = 0; i < arraySize; ++i)
        {
            RotateUsingQuaternion(transforms[i], quaternions[i]);
        }
    }
}

BENCHMARK(BM_RotateUsingQuaternion)->Range(from, to);

BENCHMARK_MAIN();