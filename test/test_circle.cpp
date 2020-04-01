#include <mathematics/circle.h>
#include <mathematics/rect.h>
#include <gtest/gtest.h>

TEST(Engine, TestCircle)
{
	const neko::Circle2D circleA(neko::Vec2f(2, 2), 2);
	const neko::Circle2D circleB(neko::Vec2f(3, 3), 3);

    const neko::Sphere3D sphere1(neko::Vec3f(2, 2, 2), 2);
    const neko::Sphere3D sphere2(neko::Vec3f(3, 3, 2), 2);

	const neko::Rect2f rect(neko::Vec2f(1, 1), neko::Vec2f(2, 2));
	
	EXPECT_TRUE(circleA.Intersects(circleB));
    EXPECT_TRUE(sphere1.Intersects(sphere2));
	EXPECT_TRUE(circleA.RectCircleIntersects(rect));
    EXPECT_TRUE(neko::Sphere3D::IsPlanCircleIntersects(sphere1, neko::Vec3f(0, 1, 0), neko::Vec3f(0, 2, 0)));
}