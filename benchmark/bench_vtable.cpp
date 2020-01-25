//
// Created by efarhan on 11/17/19.
//
#include <memory>
#include <random>
#include "mathematics/rect.h"
#include <benchmark/benchmark.h>


const int fromRange = 8;
const int toRange = 1<<15;

enum class LightType
{
    LIGHT_POSITION,
    AREA_LIGHT,
    SPOT_LIGHT
};

class Light
{
public:
    explicit Light(float range) : range_(range)
    {};

    virtual ~Light() = default;

    void parentLightBounds()
    {
        switch (lightType_)
        {
            case LightType::LIGHT_POSITION:
            {
                radius_ = range_;
                aabb_ = neko::Rect3f(neko::Vec3f::Zero, neko::Vec3f(range_));
                break;
            }
            case LightType::AREA_LIGHT:
            {
                aabb_ = neko::Rect3f(neko::Vec3f(0, 0, -range_ * 0.5f), neko::Vec3f(range_, range_, 0.5f * range_));
                break;
            }
            case LightType::SPOT_LIGHT:
            {
                neko::Rect3f aabb{neko::Vec3f(), neko::Vec3f()};
                float lenOpposite = tanHalfAngle_ * range_;
                aabb.center = neko::Vec3f(0, 0, -range_ * 0.5f);
                aabb.halfSize = neko::Vec3f(lenOpposite, lenOpposite, range_ * 0.5f);
                radius_ = aabb.GetRadius();
                aabb_ = aabb;
                break;
            }
            default:
            {
                break;
            }
        }
    }

    virtual void virtualLightBounds() = 0;
    LightType GetLightType() const { return lightType_; }
protected:
    LightType lightType_;
    float radius_ = 0.0f;
    float range_ = 0.0f;
    float tanHalfAngle_ = 0.0f;
    neko::Rect3f aabb_{neko::Vec3f::Zero, neko::Vec3f::Zero};
};

class PositionLight : public Light
{
public:
    explicit PositionLight(float range) : Light(range)
    {
        lightType_ = LightType::LIGHT_POSITION;
    };

    void virtualLightBounds() override
    {
        radius_ = range_;
        aabb_ = neko::Rect3f(neko::Vec3f::Zero, neko::Vec3f(range_));
    }

    void ownLightBounds()
    {
        radius_ = range_;
        aabb_ = neko::Rect3f(neko::Vec3f::Zero, neko::Vec3f(range_));
    }

};

class SpotLight : public Light
{
public:
    explicit SpotLight(float range, float tanHalfSize) : Light(range)
    {
        tanHalfAngle_ = tanHalfSize;
        lightType_ = LightType::SPOT_LIGHT;
    }

    void virtualLightBounds() override
    {
        neko::Rect3f aabb{neko::Vec3f(), neko::Vec3f()};
        float lenOpposite = tanHalfAngle_ * range_;
        aabb.center = neko::Vec3f(0, 0, -range_ * 0.5f);
        aabb.halfSize = neko::Vec3f(lenOpposite, lenOpposite, range_ * 0.5f);
        radius_ = aabb.GetRadius();
        aabb_ = aabb;
    }

    void ownLightBounds()
    {
        neko::Rect3f aabb{neko::Vec3f(), neko::Vec3f()};
        float lenOpposite = tanHalfAngle_ * range_;
        aabb.center = neko::Vec3f(0, 0, -range_ * 0.5f);
        aabb.halfSize = neko::Vec3f(lenOpposite, lenOpposite, range_ * 0.5f);
        radius_ = aabb.GetRadius();
        aabb_ = aabb;
    }

};

class AreaLight : public Light
{
public:
    explicit AreaLight(float range) : Light(range)
    {
        lightType_ = LightType::AREA_LIGHT;
    };

    void virtualLightBounds() override
    {
        aabb_ = neko::Rect3f(neko::Vec3f(0, 0, -range_ * 0.5f), neko::Vec3f(range_, range_, 0.5f * range_));
    }

    void ownLightBounds()
    {
        aabb_ = neko::Rect3f(neko::Vec3f(0, 0, -range_ * 0.5f), neko::Vec3f(range_, range_, 0.5f * range_));
    }

};

const float lightRangeMin = 0.5f;
const float lightRangeMax = 1000.0f;
const float halfTanMin = -6.0f;
const float halfTanMax = 6.0f;

static void BM_SeparateArrayOwnFunc(benchmark::State& state)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{lightRangeMin, lightRangeMax};
    std::uniform_real_distribution<float> distTan{halfTanMin, halfTanMax};

    std::vector<PositionLight> posLights;
    posLights.reserve(state.range(0));
    std::vector<AreaLight> areaLights;
    areaLights.reserve(state.range(0));
    std::vector<SpotLight> spotLights;
    spotLights.reserve(state.range(0));
    for (size_t i = 0; i < state.range(0); i++)
    {
        posLights.push_back(PositionLight(dist(g)));
        areaLights.push_back(AreaLight(dist(g)));
        spotLights.push_back(SpotLight(dist(g), distTan(g)));
    };

    for (auto _ : state)
    {
        for (auto& light : posLights)
        {
            light.ownLightBounds();
        }
        for (auto& light : areaLights)
        {
            light.ownLightBounds();
        }
        for (auto& light : spotLights)
        {
            light.ownLightBounds();
        }
    }
}

BENCHMARK(BM_SeparateArrayOwnFunc)->Range(fromRange, toRange);


static void BM_SeparateArrayVirtualFunc(benchmark::State& state)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{lightRangeMin, lightRangeMax};
    std::uniform_real_distribution<float> distTan{halfTanMin, halfTanMax};

    std::vector<PositionLight> posLights;
    posLights.reserve(state.range(0));
    std::vector<AreaLight> areaLights;
    areaLights.reserve(state.range(0));
    std::vector<SpotLight> spotLights;
    spotLights.reserve(state.range(0));
    for (size_t i = 0; i < state.range(0); i++)
    {
        posLights.push_back(PositionLight(dist(g)));
        areaLights.push_back(AreaLight(dist(g)));
        spotLights.push_back(SpotLight(dist(g), distTan(g)));
    };

    for (auto _ : state)
    {
        for (auto& light : posLights)
        {
            light.virtualLightBounds();
        }
        for (auto& light : areaLights)
        {
            light.virtualLightBounds();
        }
        for (auto& light : spotLights)
        {
            light.virtualLightBounds();
        }
    }
}

BENCHMARK(BM_SeparateArrayVirtualFunc)->Range(fromRange, toRange);

static void BM_ParentFunc(benchmark::State& state)
{
    std::vector<std::unique_ptr<Light>> lights;
    lights.reserve(3 * state.range(0));
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{lightRangeMin, lightRangeMax};
    std::uniform_real_distribution<float> distTan{halfTanMin, halfTanMax};
    for (size_t i = 0; i < state.range(0); i++)
    {
        lights.push_back(std::make_unique<PositionLight>(dist(g)));
        lights.push_back(std::make_unique<AreaLight>(dist(g)));
        lights.push_back(std::make_unique<SpotLight>(dist(g), distTan(g)));
    };

    std::shuffle(lights.begin(), lights.end(), g);
    for (auto _ : state)
    {
        for (auto& light : lights)
        {
            light->parentLightBounds();
            benchmark::DoNotOptimize(light);
        }
    }
}

BENCHMARK(BM_ParentFunc)->Range(fromRange, toRange);

static void BM_ParentFuncSorted(benchmark::State& state)
{
    std::vector<std::unique_ptr<Light>> lights;
    lights.reserve(3 * state.range(0));
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{lightRangeMin, lightRangeMax};
    std::uniform_real_distribution<float> distTan{halfTanMin, halfTanMax};
    for (size_t i = 0; i < state.range(0); i++)
    {
        lights.push_back(std::make_unique<PositionLight>(dist(g)));
        lights.push_back(std::make_unique<AreaLight>(dist(g)));
        lights.push_back(std::make_unique<SpotLight>(dist(g), distTan(g)));
    };

    std::sort(lights.begin(), lights.end(), [](const std::unique_ptr<Light>& l1, const std::unique_ptr<Light>& l2){
        return l1->GetLightType() > l2->GetLightType();
    });
    for (auto _ : state)
    {
        for (auto& light : lights)
        {
            light->parentLightBounds();

            benchmark::DoNotOptimize(light);
        }
    }
}

BENCHMARK(BM_ParentFuncSorted)->Range(fromRange, toRange);


static void BM_VirtualCall(benchmark::State& state)
{
    std::vector<std::unique_ptr<Light>> lights;
    lights.reserve(3 * state.range(0));
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{lightRangeMin, lightRangeMax};
    std::uniform_real_distribution<float> distTan{halfTanMin, halfTanMax};
    for (size_t i = 0; i < state.range(0); i++)
    {
        lights.push_back(std::make_unique<PositionLight>(dist(g)));
        lights.push_back(std::make_unique<AreaLight>(dist(g)));
        lights.push_back(std::make_unique<SpotLight>(dist(g), distTan(g)));
    };

    std::shuffle(lights.begin(), lights.end(), g);
    for (auto _ : state)
    {
        for (auto& light : lights)
        {
            light->virtualLightBounds();

            benchmark::DoNotOptimize(light);
        }
    }
}

BENCHMARK(BM_VirtualCall)->Range(fromRange, toRange);

static void BM_VirtualCallSorted(benchmark::State& state)
{
    std::vector<std::unique_ptr<Light>> lights;
    lights.reserve(3 * state.range(0));
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{lightRangeMin, lightRangeMax};
    std::uniform_real_distribution<float> distTan{halfTanMin, halfTanMax};
    for (size_t i = 0; i < state.range(0); i++)
    {
        lights.push_back(std::make_unique<PositionLight>(dist(g)));
        lights.push_back(std::make_unique<AreaLight>(dist(g)));
        lights.push_back(std::make_unique<SpotLight>(dist(g), distTan(g)));
    };

    std::sort(lights.begin(), lights.end(), [](const std::unique_ptr<Light>& l1, const std::unique_ptr<Light>& l2){
        return l1->GetLightType() > l2->GetLightType();
    });
    for (auto _ : state)
    {
        for (auto& light : lights)
        {
            light->virtualLightBounds();
            benchmark::DoNotOptimize(light);

        }
    }
}

BENCHMARK(BM_VirtualCallSorted)->Range(fromRange, toRange);