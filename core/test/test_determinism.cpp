#include "engine/log.h"
#include "gtest/gtest.h"
#include "mathematics/checksum.h"
#include "mathematics/vector.h"

#include <fmt/format.h>

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
  logDebug(fmt::format("Float checksum: {}", checksum));
}