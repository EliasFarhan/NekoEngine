#include "engine/log.h"
#include "gtest/gtest.h"
#include "mathematics/checksum.h"
#include "mathematics/vector.h"
#include "cnl/scaled_integer.h"

TEST(CompNet, FloatDeterminism)
{
    neko::Vec2f p{ 1.0f, 2.0f };
    p /= 2.0f;
    p.Rotate(neko::degree_t(90.0f));
    logDebug(std::to_string(neko::Checksum<std::uint8_t>(p)));
}

TEST(CompNet, FixedDeterminism)
{
    neko::Vec2<cnl::scaled_integer<int, cnl::power<-16>>>p
    {
        1.0f,
        2.0f
    };
    p /= 2.0f;
    p.Rotate(neko::degree_t(90.0f));
    logDebug(std::to_string(neko::Checksum<std::uint8_t>(p)));
}