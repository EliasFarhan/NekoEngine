//
// Created by efarhan on 09.02.20.
//

#include "mathematics/const.h"
#include <gtest/gtest.h>
#include "mathematics/angle.h"


TEST(Engine, TestUnits)
{
    const neko::Radian angle{neko::PI};
    const neko::Degree angle2 = angle;

    std::cout << "This should be 180: "<< angle2.GetValue()<<'\n';
    EXPECT_TRUE(std::abs(angle2.GetValue()-180.0f) < 0.001f);
}
