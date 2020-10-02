#include "engine/log.h"
#include "gtest/gtest.h"
#include "mathematics/checksum.h"
#include "mathematics/vector.h"
#include "cnl/scaled_integer.h"

TEST(CompNet, FloatDeterminism)
{
  std::uint8_t checksum = 0;
  for (float i = 0.0f; i < 100.0f; i += 1.5f)
  {
    neko::Vec2f p{ 1.5f + i, 2.5f - i };
    p /= 2.3f;
    p *= 0.02f;
    p.Rotate(neko::degree_t(90.0f));
    checksum += neko::Checksum<std::uint8_t>(p);
  }
  logDebug("Float checksum: " + std::to_string(checksum));
}

TEST(CompNet, FixedDeterminism)
{
  std::uint8_t checksum = 0;
  for (float i = 0.0f; i < 100.0f; i += 1.5f)
  {
    neko::Vec2<cnl::scaled_integer<int, cnl::power<-16>>> p
    {
        1.5f + i,
        2.5f - i
    };
    p /= 2.3f;
    p *= 0.02f;
    p.Rotate(neko::degree_t(90.0f));
    checksum += neko::Checksum<std::uint8_t>(p);
  }
  logDebug("Fixed checksum: " + std::to_string(checksum));
}