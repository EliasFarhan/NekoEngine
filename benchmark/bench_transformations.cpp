#include <iostream>
#include <benchmark/benchmark.h>
#include <mathematics/transform.h>

using namespace neko;

const long from = 2;
const long to = 1024;

const Transform3d& Translate(const Transform3d& mat, const Vec3f translation)
{
    return Transform3d::Translate(mat, translation);
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

const Transform3d& Scale(const Transform3d& mat, const Vec3f scale)
{
    return Transform3d::Scale(mat, scale);
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

const Transform3d& RotateAroundAxisDegrees(const Transform3d& mat, const degree_t angle, const Vec3f axis)
{
    return Transform3d::Rotate(mat, angle, axis);
}

static void BM_RotateAroundAxisDegrees(benchmark::State& s)
{
    int arraySize = s.range(0);
    std::vector<Transform3d> transforms(arraySize);
    std::vector<Vec3f> axes(arraySize);
    std::vector<degree_t> angles(arraySize);

    for (int i = 0; i < arraySize; ++i)
    {
        axes[i] = Vec3f(rand(), rand(), rand());
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

const Transform3d& RotateAroundAxisRadians(const Transform3d& mat, const radian_t angle, const Vec3f axis)
{
    return Transform3d::Rotate(mat, angle, axis);
}

static void BM_RotateAroundAxisRadians(benchmark::State& s)
{
    int arraySize = s.range(0);
    std::vector<Transform3d> transforms(arraySize);
    std::vector<Vec3f> axes(arraySize);
    std::vector<radian_t> angles(arraySize);

    for (int i = 0; i < arraySize; ++i)
    {
        axes[i] = Vec3f(rand(), rand(), rand());
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

const Transform3d& RotateUsingCardinals(const Transform3d& mat, const EulerAngles angles)
{
    return Transform3d::Rotate(mat, angles);
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

const Transform3d& RotateUsingQuaternion(const Transform3d& mat, const Quaternion q)
{
    return Transform3d::Rotate(mat, q);
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