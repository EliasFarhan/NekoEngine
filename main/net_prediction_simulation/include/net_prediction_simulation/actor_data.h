#pragma once
#include <cstdlib>
#include <engine/globals.h>
#include <engine/entity.h>
#include <mathematics/vector.h>

namespace neko::net
{
struct ActorData
{
    neko::Index tickIndex = neko::INVALID_INDEX;
    neko::Entity entity = neko::INVALID_ENTITY;
    neko::Vec2f position{};
    neko::Vec2f velocity{};
};
const size_t serverActorDataBufferSize = 4;
const neko::EntityMask velocityMask = 1u << 15u;


enum class ServerPredictionType
{
    None,
    Interpolation,
    Extrapolation,
    Catmull_Interpolation
};

enum class ClientMovementType
{
    Linear,
    Planet,
    Boids
};


const size_t actorNmb = 100;
const float linearSpeed = 2.0f;
const neko::Vec2f points[] = {
        neko::Vec2f(0.0f, -0.2f),
        neko::Vec2f(0.2f, 0.2f),
        neko::Vec2f(-0.2f, 0.2f)
};

const float gravityConst = 1.0f;
const float centerMass = 10.0f;
const float planetMass = 1.0f;
}