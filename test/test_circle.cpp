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
	
	auto start = std::chrono::steady_clock::now();

	std::cout << neko::Circle::Sqrt(2.0f) << '\n';
	std::cout << neko::Circle::Sqrt(4.0f) << '\n';
	
	EXPECT_TRUE(circleA.Intersects(circleB));
	EXPECT_TRUE(circleB.Intersects(circleC));
	EXPECT_TRUE(circleA.Intersects(circleD));
	EXPECT_FALSE(circleC.Intersects(circleD));
	
	auto end = std::chrono::steady_clock::now();

	auto diff = end - start;
	std::cout << std::chrono::duration <double, std::nano>(diff).count() << " ns\n";
	EXPECT_TRUE(sphere1.Intersects(sphere2));

	EXPECT_TRUE(circleA.RectCircleIntersects(rect));
	EXPECT_TRUE(sphere1.IsPlanSphereIntersects(plan));
	
}

TEST(Engine, TestNCircle)
{
	
	neko::Vec2f pos(2.0f, 2.0f);
	neko::Vec2f pos2(0.0f, 0.0f);

	std::array<neko::Circle, 4> array;
	std::array<neko::Circle, 4> array2;
	
	/*
	array = { 	 neko::Circle (neko::Vec2f(2, 2), 2),
				 neko::Circle (neko::Vec2f(3, 3), 3),
				 neko::Circle (neko::Vec2f(3, 3), 1),
				 neko::Circle (neko::Vec2f(-2, 2), 2) };
	
	array2 = { neko::Circle(neko::Vec2f(5.0f, 6.0f), 4.0f),
			   neko::Circle(neko::Vec2f(7.0f, 8.0f), 4.0f),
			   neko::Circle(neko::Vec2f(9.0f, 10.0f), 4.0f),
			   neko::Circle(neko::Vec2f(9.0f, 10.0f), 4.0f) };
	*/
			  
	for(size_t i = 0; i < 4; i++)
	{

		array[i] = neko::Circle(pos, 4.0f);
		std::cout << array[i].center << '\n';
		pos += neko::Vec2f(3.0f,3.0f);
	
	
		array2[i] = neko::Circle(pos2, 4.0f);
		std::cout << array2[i].center << '\n';
		pos2 += neko::Vec2f(3.0f,3.0f);
	}

	const neko::FourCircle n_circle(array);
	const neko::FourCircle n_circle2(array2);
	
	const auto result = n_circle.IntersectsIntrinsics(n_circle2);
	auto start = std::chrono::steady_clock::now();
	const auto results = n_circle.IntersectsCircleIntrinsics(array2[2] , 4);
	auto end = std::chrono::steady_clock::now();
	

	std::cout << "---------TEST INTERSECT INTRINSICS---------\n";

	for (auto& tet : result)
	{
		std::cout << tet << "\n";
		EXPECT_TRUE(tet == array[tet].Intersects(array2[tet]));
	}
	/*auto diff = end - start;
	std::cout << std::chrono::duration <double, std::nano>(diff).count() << " ns\n";
	EXPECT_TRUE(results[0] == array[0].Intersects(array2[2]));
	EXPECT_TRUE(results[1] == array[1].Intersects(array2[2]));
	EXPECT_TRUE(results[2] == array[2].Intersects(array2[2]));
	EXPECT_TRUE(results[3] == array[3].Intersects(array2[2]));*/
	std::cout << "---------TEST INTERSECT CIRCLE INTRINSICS---------\n";

	for (auto& tet : results)
	{
		std::cout << tet << "\n";
		EXPECT_TRUE(tet == array[tet].Intersects(array2[2]));
	}
	/*EXPECT_TRUE(result[0] == array[1].Intersects(array[0]));
	EXPECT_TRUE(result[1] == array[2].Intersects(array[2]));
	EXPECT_TRUE(result[2] == array[1].Intersects(array[3]));
	EXPECT_FALSE(result[3] == array[3].Intersects(array[3]));*/
}


TEST(Engine, TestNSphere)
{

	neko::Vec3f pos(2.0f, 2.0f, 2);
	neko::Vec3f pos2(0.0f, 0.0f,0);

	std::array<neko::Sphere, 4> array;
	std::array<neko::Sphere, 4> array2;
	std::array<neko::Plan, 4> plans;
	
	neko::Plan plan(neko::Vec3f(5.0f, 6.0f, 2), neko::Vec3f(0, 0, 5));
			   
	
	for(size_t i = 0; i < 4; i++)
	{
		array[i] = neko::Sphere(pos2, 4.0f);
		std::cout << array[i].center << '\n';
		pos2 += neko::Vec3f(3.0f,3.0f, 3);
	
		array2[i] = neko::Sphere(pos, 4.0f);
		std::cout << array2[i].center << '\n';
		pos += neko::Vec3f(3.0f,3.0f, 3);
	}

	neko::FourSphere n_sphere(array);
	neko::FourSphere n_sphere2(array2);
	
	//auto result = n_sphere.Intersects(n_sphere);
	auto result_test = n_sphere.Intersects(n_sphere2);
	auto result = n_sphere.IntersectIntrinsics(n_sphere2);

	auto results = n_sphere.IntersectSphereIntrinsics(array2[2]);	
	
    auto plan_result = n_sphere.IntersectSpherePlanIntrinsics(plan);
	
	std::cout << "---------TEST INTERSECT INTRINSICS---------\n";
	for (auto& tet : result)
	{
		std::cout << tet << "\n";
		EXPECT_TRUE(tet == array[tet].Intersects(array2[tet]));

	}
	std::cout << "---------TEST INTERSECTS INTRINSICS SPHERE---------\n";

	for (auto& tet : results)
	{
		std::cout << tet << "\n";
		EXPECT_TRUE(tet == array[tet].Intersects(array2[tet]));

	}
	std::cout << "---------TEST INTERSECTS PLAN INTRINSICS---------\n";

	for (auto& tet : plan_result)
	{
		std::cout << plans[tet].pos << '\n';
		std::cout << tet << "\n";
		EXPECT_TRUE(tet == array[tet].IsPlanSphereIntersects(plan));

	}
}

