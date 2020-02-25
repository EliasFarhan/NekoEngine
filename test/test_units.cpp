//
// Created by efarhan on 09.02.20.
//

#include "mathematics/const.h"
#include <gtest/gtest.h>
#include "mathematics/angle.h"


TEST(Engine, TestUnits)
{
    neko::radian_t angle{neko::PI};
    neko::degree_t angle2 = angle;

    std::cout << "This should be 180: "<< angle2.value()<<'\n';
    EXPECT_TRUE(std::abs(angle2.value()-180.0f) < 0.001f);
}
