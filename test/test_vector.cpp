//
// Created by frousties on 11/02/2020.
//

#include <gtest/gtest.h>
#include <iostream>

#include <mathematics/vector_nvec.h>

namespace neko
{
TEST(Engine, TestVector)
{
	std::array<Vec3f, 4> array;
	array.fill(Vec3f(5, 1, 3));
	FourVec3f fourVec3(array);

	auto results1 = fourVec3.MagnitudeIntrinsics();
	auto results2 = fourVec3.SquareMagnitudeIntrinsics();
	auto results3 = FourVec3f::DotIntrinsics(fourVec3, fourVec3);
	auto results4 = FourVec3f::ReflectIntrinsics(fourVec3, fourVec3);
	const auto results4Base = Vec3f::Reflect(array[0], array[0]);

	for (auto& result : results1)
	{
		std::cout << result << "\n";
		EXPECT_TRUE(result - array[0].Magnitude() < 0.01f);
	}
	for (auto& result : results2)
	{
		std::cout << result << "\n";
		EXPECT_TRUE(result - array[0].SquareMagnitude() < 0.01f);
	}
	for (auto& result : results3)
	{
		std::cout << result << "\n";
		EXPECT_TRUE(result - Vec3f::Dot(array[0], array[0]) < 0.01f);
	}
	for (int i = 0; i < 4; ++i)
	{
		auto result = Vec3f(results4.xs[i], results4.ys[i], results4.zs[i]);
		std::cout << result << "\n";
		EXPECT_TRUE((result - results4Base).x < 0.01f);
		EXPECT_TRUE((result - results4Base).y < 0.01f);
		EXPECT_TRUE((result - results4Base).z < 0.01f);
	}
}
}
