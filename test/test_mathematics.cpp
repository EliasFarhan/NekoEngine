//
// Created by efarhan on 11/16/19.
//
#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <random>
#include <gtest/gtest.h>
#include <mathematics/func_table.h>

#include <mathematics/quaternion.h>
#include <mathematics/matrix.h>
#include "mathematics/vector.h"


const float maxNmb = 100.0f;

void RandomFill(std::vector<float>& numbers, float start = -maxNmb, float end = maxNmb)
{
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_real_distribution<float> dist{ start, end };
	std::generate(numbers.begin(), numbers.end(), [&g, &dist]() {return dist(g); });
}

TEST(Engine, TestSinTable)
{
	neko::FuncTable<float> sinFuncTable(0.0f, neko::PI, [](float x) { return neko::Sin(neko::radian_t(x)); });
	sinFuncTable.GenerateTable();
	const size_t sampleSize = 1024;
	std::vector<float> localNumbers(sampleSize);
	RandomFill(localNumbers, 0.0f, M_PI);

	float error = 0.0f;
	for (auto v : localNumbers)
	{
		error += std::abs(sinFuncTable.GetValue(v) - neko::Sin(neko::radian_t(v)));
	}
	error /= float(sampleSize);
	EXPECT_LT(error, 0.01f);
}

TEST(Engine, TestQuaternion)
{
	//Variables
	neko::Quaternion quaternionA = neko::Quaternion(0.71, 0, 0, 0.71);
	neko::Quaternion quaternionB = neko::Quaternion(0, 0, 0, 1);
	neko::Quaternion quaternionACopy;

	//Display start variables
	std::cout << "QuaternionA" << quaternionA << '\n';
	std::cout << std::endl;
	std::cout << "QuaternionB" << quaternionB << '\n';
	std::cout << std::endl << std::endl;

	//Dot Product Test
	std::cout << "Dot product: " << neko::Quaternion::Dot(quaternionA, quaternionB) << std::endl << std::endl;

	//Normalize Test
	std::cout << "Normalize: ";
	std::cout << "NormalizedQuaternionA" << neko::Quaternion::Normalized(quaternionA) << '\n';
	std::cout << std::endl << std::endl;

	//Magnitude Test
	std::cout << "Magnitude: " << neko::Quaternion::Magnitude(quaternionA) << '\n' << '\n';

	//AngleAxis Test
	std::cout << "AngleAxis: " << "Cannot be tested right now" << '\n' << '\n';

	//Angle Test
	std::cout << "Angle: " << neko::Quaternion::Angle(quaternionA, quaternionB) << '\n' << '\n';

	//Conjugate Test
	quaternionACopy = quaternionA;
	std::cout << "Conjugate: ";
	std::cout << "QuaternionAConjugate" << quaternionACopy.Conjugate() << '\n';
	std::cout << '\n' << '\n';

	//Inverse Test
	quaternionACopy = quaternionA;
	std::cout << "Inverse: ";
	std::cout << "QuaternionAInverse" << quaternionACopy.Inverse() << '\n';
	std::cout << std::endl << std::endl;

	//FromEuler
	std::cout << "Euler: " << "Cannot be tested right now" << '\n' << '\n';
}
TEST(Engine, TestMatrix4)
{
	neko::Mat4f m1(std::array<neko::Vec4f, 4>
	{
		neko::Vec4f{ 1,2,3,4 },
			neko::Vec4f{ -1,-2,-3,-4 },
			neko::Vec4f{ 4,2,2,1 },
			neko::Vec4f{ -4,-3,-2,-1 }
	});

	neko::Mat4f result = neko::Mat4f(std::array<neko::Vec4f, 4>{
		neko::Vec4f(-5, 5, 6, -5),
			neko::Vec4f(-8, 8, 5, -3),
			neko::Vec4f(-5, 5, 8, -5),
			neko::Vec4f(-5, 5, 9, -5)
	});
	result = result.Transpose();
	EXPECT_LT(neko::Mat4f::MatrixDifference(m1.MultiplyNaive(m1), result), 0.01f);
	EXPECT_LT(neko::Mat4f::MatrixDifference(m1.MultiplyIntrinsincs(m1), result), 0.01f);
	EXPECT_LT(neko::Mat4f::MatrixDifference(m1.MultiplyNaive(m1), m1.MultiplyIntrinsincs(m1)), 0.01f);
}