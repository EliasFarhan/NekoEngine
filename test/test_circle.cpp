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

	const neko::Rect rect(neko::Vec2f(1, 1), neko::Vec2f(2, 2));
	const neko::Plan plan(neko::Vec3f(0, 2, 0), neko::Vec3f(0, 1, 0));
	

	std::cout << neko::Circle::Sqrt(2.0f) << '\n';
	std::cout << neko::Circle::Sqrt(4.0f) << '\n';
	
	EXPECT_TRUE(circleA.Intersects(circleB)); // 01
	EXPECT_TRUE(circleB.Intersects(circleC));// 12
	EXPECT_TRUE(circleA.Intersects(circleD));//03
	EXPECT_FALSE(circleC.Intersects(circleD));//23
	
	auto end = std::chrono::steady_clock::now();

	EXPECT_TRUE(sphere1.Intersects(sphere2));

	EXPECT_TRUE(circleA.RectCircleIntersects(rect));
	EXPECT_TRUE(sphere1.IsPlanSphereIntersects(plan));
	
}

TEST(Engine, TestNCircle)
{
	
	neko::Vec2f pos(2.0f, 2.0f);
	neko::Vec2f pos2(0.0f, 0.0f);	
	
	std::array<neko::Circle, 4> array = {
		neko::Circle(neko::Vec2f(2, 2), 2),
		neko::Circle(neko::Vec2f(3, 3), 3),
		neko::Circle(neko::Vec2f(3, 3), 1),
		neko::Circle(neko::Vec2f(-2, 2), 2)
	};

	/*		  
	for(size_t i = 0; i < 4; i++)
	{

		array[i] = neko::Circle(pos, 4.0f);
		std::cout << array[i].center << '\n';
		pos += neko::Vec2f(3.0f,3.0f);
	
	
		array2[i] = neko::Circle(pos2, 4.0f);
		std::cout << array2[i].center << '\n';
		pos2 += neko::Vec2f(3.0f,3.0f);
	}
*/
	const neko::FourCircle n_circle(array);
	
	const auto results = n_circle.IntersectsIntrinsics(n_circle);
	std::cout << "---------TEST INTERSECT INTRINSICS---------\n";
/*
	for (auto& tet : result)
	{
		std::cout << tet << "\n";
		EXPECT_TRUE(tet == array[tet].Intersects(array2[tet]));
	}*/
	EXPECT_TRUE(results[0] == array[0].Intersects(array[1]));
	EXPECT_TRUE(results[1] == array[1].Intersects(array[2]));
	EXPECT_TRUE(results[2] == array[0].Intersects(array[3]));
	EXPECT_FALSE(results[3] == array[2].Intersects(array[3]));

	for (auto& tet : results)
	{
		std::cout << tet << "\n";
	}
}


TEST(Engine, TestNSphere)
{

	

	std::array<neko::Sphere, 4> array = {
		neko::Sphere(neko::Vec3f(2, 2, 2), 2),
		neko::Sphere(neko::Vec3f(3, 3, 0), 3),
		neko::Sphere(neko::Vec3f(3, 3, 3), 1),
		neko::Sphere(neko::Vec3f(-2, -2, -2), 2)
	};
	neko::Sphere sphereTest(neko::Vec3f(0, 0, 0), 4);
	
	std::array<neko::Sphere, 4> array2;
	array.fill(sphereTest);
	std::array<neko::Plan, 4> plans;
	
	neko::Plan plan(neko::Vec3f(5.0f, 6.0f, 2), neko::Vec3f(0, 0, 5));
			   
	
	
	neko::FourSphere n_sphere(array);
	neko::FourSphere n_sphere2(array2);
	
	//auto result = n_sphere.Intersects(n_sphere);
	auto result_test = n_sphere.Intersects(n_sphere2);
	auto result = n_sphere.IntersectIntrinsics(n_sphere);
	
	std::cout << "---------TEST INTERSECT INTRINSICS---------\n";
	
	EXPECT_TRUE(result[0] == array[0].Intersects(array[1]));
	EXPECT_TRUE(result[1] == array[1].Intersects(array[2]));
	EXPECT_TRUE(result[2] == array[0].Intersects(array[3]));
	EXPECT_FALSE(result[3] == array[2].Intersects(array[3]));
	for (auto& tet : result)
	{
		std::cout << tet << "\n";
	}
	std::cout << "---------TEST INTERSECTS FOUR SPHERE---------\n";
	for (auto& tet : result_test)
	{
		std::cout << tet << "\n";
		EXPECT_TRUE(tet == array[tet].Intersects(array2[tet]));
	}

}

