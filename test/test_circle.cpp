#include <mathematics/circle.h>
#include <mathematics/rect.h>
#include <gtest/gtest.h>

TEST(Engine, TestCircle)
{
	const neko::Circle circleA(neko::Vec2f(2, 2), 2);
	const neko::Circle circleB(neko::Vec2f(3, 3), 3);
	const neko::Circle circleC(neko::Vec2f(3, 3), 1);
	const neko::Circle circleD(neko::Vec2f(-2, 2), 2);

	const neko::Sphere sphere1(neko::Vec3f(2, 2, 2), 2);
	const neko::Sphere sphere2(neko::Vec3f(3, 3, 2), 2);

	std::cout << neko::Sqrt(2.0f) << '\n';
	std::cout << neko::Sqrt(4.0f) << '\n';
	
	EXPECT_TRUE(circleA.Intersects(circleB));
	EXPECT_TRUE(circleB.Intersects(circleC));
	EXPECT_TRUE(circleA.Intersects(circleD));
	EXPECT_FALSE(circleC.Intersects(circleD));
}

TEST(Engine, TestSphere)
{
	const neko::Sphere sphereA(neko::Vec3f(2, 2, 2), 2);
	const neko::Sphere sphereB(neko::Vec3f(3, 3, 0), 3);
	const neko::Sphere sphereC(neko::Vec3f(3, 3, 3), 1);
	const neko::Sphere sphereD(neko::Vec3f(-2, -2, -2), 2);

	const neko::Rect rect(neko::Vec2f(1, 1), neko::Vec2f(2, 2));
	const neko::Plan plan(neko::Vec3f(0, 2, 0), neko::Vec3f(0, 1, 0));
	

	std::cout << neko::Sqrt(2.0f) << '\n';
	std::cout << neko::Sqrt(4.0f) << '\n';
	EXPECT_TRUE(sphereA.Intersects(sphereB));
	EXPECT_TRUE(sphereB.Intersects(sphereC));
	EXPECT_TRUE(sphereA.Intersects(sphereD));
	EXPECT_FALSE(sphereC.Intersects(sphereD));
}

TEST(Engine, TestFourCircle)
{
	
	neko::Vec2f pos(2.0f, 2.0f);
	neko::Vec2f pos2(0.0f, 0.0f);	
	
	std::array<neko::Circle, 4> array1 = {
		neko::Circle(neko::Vec2f(2, 2), 2),
		neko::Circle(neko::Vec2f(3, 3), 3),
		neko::Circle(neko::Vec2f(3, 3), 1),
		neko::Circle(neko::Vec2f(-2, 2), 2)
	};
	std::array<neko::Circle, 4> array2 = {
	neko::Circle(neko::Vec2f(3, 3), 1),
	neko::Circle(neko::Vec2f(-2, 2), 2),
	neko::Circle(neko::Vec2f(2, 2), 2),
	neko::Circle(neko::Vec2f(3, 3), 3),
	};
	const neko::FourCircle n_circle1(array1);
	const neko::FourCircle n_circle2(array2);
	
	const auto results = n_circle1.IntersectsIntrinsics(n_circle2);
	std::cout << "---------TEST INTERSECT INTRINSICS---------\n";

	EXPECT_TRUE(results[0] == array1[0].Intersects(array2[0]));
	EXPECT_TRUE(results[1] == array1[1].Intersects(array2[1]));
	EXPECT_TRUE(results[2] == array1[2].Intersects(array2[2]));
	EXPECT_TRUE(results[3] == array1[3].Intersects(array2[3]));

	
}


TEST(Engine, TestFourSphere)
{
	std::array<neko::Sphere, 4> array1 = {
		neko::Sphere(neko::Vec3f(2, 2, 2), 2),
		neko::Sphere(neko::Vec3f(3, 3, 3), 3),
		neko::Sphere(neko::Vec3f(3, 3, 3), 1),
		neko::Sphere(neko::Vec3f(2, 2, -2), 2)
	};
	std::array<neko::Sphere, 4> array2 = {
	neko::Sphere(neko::Vec3f(2, 2, -2), 2),
	neko::Sphere(neko::Vec3f(2, 2, 2), 2),
	neko::Sphere(neko::Vec3f(3, 3, 3), 1),
	neko::Sphere(neko::Vec3f(3, 3, 3), 3),
	};

	const neko::FourSphere n_sphere(array1);
	const neko::FourSphere n_sphere2(array2);
	
	auto result = n_sphere.IntersectIntrinsics(n_sphere);
	
	std::cout << "---------TEST INTERSECT INTRINSICS---------\n";
	
	EXPECT_TRUE(result[0] == array1[0].Intersects(array2[0]));
	EXPECT_TRUE(result[1] == array1[1].Intersects(array2[1]));
	EXPECT_TRUE(result[2] == array1[2].Intersects(array2[2]));
	EXPECT_TRUE(result[3] == array1[3].Intersects(array2[3]));
	
}

