#include <mathematics/circle.h>
#include <mathematics/rect.h>
#include <gtest/gtest.h>

/*TEST(Engine, TestCircle)
{
	neko::Circle circleA(neko::Vec2f(2, 2), 2);
	neko::Circle circleB(neko::Vec2f(3, 3), 3);
	neko::Circle circleC(neko::Vec2f(3, 3), 1);
	neko::Circle circleD(neko::Vec2f(-2, 2), 2);

	neko::Sphere sphere1(neko::Vec3f(2, 2, 2), 2);
	neko::Sphere sphere2(neko::Vec3f(3, 3, 2), 3);
	neko::Sphere sphere3(neko::Vec3f(3, 3, 2), 1);
	neko::Sphere sphere4(neko::Vec3f(-2, 2, 2), 2);

	neko::Rect2f rect(neko::Vec2f(1, 1), neko::Vec2f(2, 2));
	neko::Plan plan(neko::Vec3f(0, 2, 0), neko::Vec3f(0, 1, 0));

	EXPECT_TRUE(circleA.Intersects(circleB));
	EXPECT_TRUE(circleB.Intersects(circleC));
	EXPECT_TRUE(circleA.Intersects(circleD));
	EXPECT_FALSE(circleC.Intersects(circleD));

	EXPECT_TRUE(sphere1.Intersects(sphere2));

	EXPECT_TRUE(circleA.RectCircleIntersects(rect));
}*/
TEST(Engine, TestCircle)
{
	const neko::Circle circleA(neko::Vec2f(2, 2), 2);
	const neko::Circle circleB(neko::Vec2f(3, 3), 3);
	const neko::Circle circleC(neko::Vec2f(3, 3), 1);
	const neko::Circle circleD(neko::Vec2f(-2, 2), 2);

	const neko::Sphere sphere1(neko::Vec3f(2, 2, 2), 2);
	const neko::Sphere sphere2(neko::Vec3f(3, 3, 2), 2);

	const neko::Rect2f rect(neko::Vec2f(1, 1), neko::Vec2f(2, 2));
	const neko::Plan plan(neko::Vec3f(0, 2, 0), neko::Vec3f(0, 1, 0));
	
	EXPECT_TRUE(circleA.Intersects(circleB));
	EXPECT_TRUE(circleB.Intersects(circleC));
	EXPECT_TRUE(circleA.Intersects(circleD));
	EXPECT_FALSE(circleC.Intersects(circleD));

	EXPECT_TRUE(sphere1.Intersects(sphere2));

	EXPECT_TRUE(circleA.RectCircleIntersects(rect));
	EXPECT_TRUE(sphere1.IsPlanSphereIntersects(plan));
}
/*TEST(Engine, TestNCircle)
{	
	std::array<neko::Sphere, 4> array;
	array.fill(neko::Sphere(neko::Vec3f(3, 4,3), 4));

	neko::NSphere n_circle(array);
	auto result = n_circle.Intersect(array);

	for (auto& tet : result) {
		std::cout << tet << "\n";
		EXPECT_TRUE(tet - array[0].Intersects(array[0]) < 0.01f);
	}
	
}*/