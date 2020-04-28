#include <mathematics/circle.h>
#include <mathematics/rect.h>
#include <gtest/gtest.h>

TEST(Engine, TestCircle)
{
	const neko::Circle circleA(neko::Vec2f(2, 2), 2);
	const neko::Circle circleB(neko::Vec2f(3, 3), 3);
	const neko::Circle circleC(neko::Vec2f(3, 3), 1);
	const neko::Circle circleD(neko::Vec2f(-2, 2), 2);

	EXPECT_TRUE(circleA.Intersects(circleB));
	EXPECT_TRUE(circleB.Intersects(circleC));
	EXPECT_TRUE(circleA.Intersects(circleD));
	EXPECT_FALSE(circleC.Intersects(circleD));
}
TEST(Engine, TestCircleRSqrt)
{
	const neko::Circle circleA(neko::Vec2f(2, 2), 2);
	const neko::Circle circleB(neko::Vec2f(3, 3), 3);
	const neko::Circle circleC(neko::Vec2f(3, 3), 1);
	const neko::Circle circleD(neko::Vec2f(-2, 2), 2);

	EXPECT_TRUE(circleA.IntersectsOther(circleB));
	EXPECT_TRUE(circleB.IntersectsOther(circleC));
	EXPECT_TRUE(circleA.IntersectsOther(circleD));
	EXPECT_FALSE(circleC.IntersectsOther(circleD));
}

TEST(Engine, TestSphere)
{
	const neko::Sphere sphereA(neko::Vec3f(2, 2, 2), 2);
	const neko::Sphere sphereB(neko::Vec3f(2, 2, -2), 2);
	const neko::Sphere sphereC(neko::Vec3f(2, 0, 0), 1);
	const neko::Sphere sphereD(neko::Vec3f(0, 3, 0), 2);

	EXPECT_TRUE(sphereA.Intersects(sphereB));
	EXPECT_TRUE(sphereB.Intersects(sphereC));
	EXPECT_TRUE(sphereA.Intersects(sphereD));
	EXPECT_FALSE(sphereC.Intersects(sphereD));
}

TEST(Engine, TestSphereRSqrt)
{
	const neko::Sphere sphereA(neko::Vec3f(2, 2, 2), 2);
	const neko::Sphere sphereB(neko::Vec3f(2, 2, -2), 2);
	const neko::Sphere sphereC(neko::Vec3f(2, 0, 0), 1);
	const neko::Sphere sphereD(neko::Vec3f(0, 3, 0), 2);

	EXPECT_TRUE(sphereA.IntersectsRSqrt(sphereB));
	EXPECT_TRUE(sphereB.IntersectsRSqrt(sphereC));
	EXPECT_TRUE(sphereA.IntersectsRSqrt(sphereD));
	EXPECT_FALSE(sphereC.IntersectsRSqrt(sphereD));
}

TEST(Engine, TestFourCircle)
{
	std::array<neko::Circle, 4> array = {
		neko::Circle(neko::Vec2f(-2, 0), 1),
		neko::Circle(neko::Vec2f(3, 6), 3),
		neko::Circle(neko::Vec2f(2, -7), 1),
		neko::Circle(neko::Vec2f(4, 1), 2)
	};
	std::array<neko::Circle, 4> array2 = {
		neko::Circle(neko::Vec2f(-2, 0), 2),
		neko::Circle(neko::Vec2f(3, 0), 3),
		neko::Circle(neko::Vec2f(2, 2), 1),
		neko::Circle(neko::Vec2f(1,0), 2)
	};
	const neko::FourCircle n_circle(array);
	const neko::FourCircle n_circle2(array2);
	
	const auto results = neko::FourCircle::IntersectsIntrinsicsCircle(n_circle, n_circle2);
	EXPECT_TRUE(results & array[0].Intersects(array2[0]));
	EXPECT_TRUE(results & array[1].Intersects(array2[1]));
	EXPECT_FALSE(results & array[2].Intersects(array2[2]));
	EXPECT_TRUE(results & array[3].Intersects(array2[3]));
}


TEST(Engine, TestFourSphere)
{
	std::array<neko::Sphere, 4> array = {
		neko::Sphere(neko::Vec3f(-3, 0, 0), 2),
		neko::Sphere(neko::Vec3f(3, 3, 0), 3),
		neko::Sphere(neko::Vec3f(2, 0, 0), 1),
		neko::Sphere(neko::Vec3f(5, 0, 0), 2)
	};
	
	std::array<neko::Sphere, 4> array2 = {
		neko::Sphere(neko::Vec3f(-3, 0, 0), 2),
		neko::Sphere(neko::Vec3f(3, 3, 6), 3),
		neko::Sphere(neko::Vec3f(2, 0, -7), 1),
		neko::Sphere(neko::Vec3f(5,-2, 2), 2)
	};
	
	const neko::FourSphere n_sphere(array);
	const neko::FourSphere n_sphere2(array2);

	const auto results = neko::FourSphere::IntersectIntrinsicsSphere(n_sphere, n_sphere2);

	EXPECT_TRUE(results & array[0].Intersects(array2[0]));
	EXPECT_TRUE(results & array[1].Intersects(array2[1]));
	EXPECT_FALSE(results & array[2].Intersects(array2[2]));
	EXPECT_TRUE(results & array[3].Intersects(array2[3]));
}
