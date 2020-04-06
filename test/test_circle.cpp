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

	EXPECT_TRUE(circleA.Intersects(circleB);
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
	
	array = { neko::Circle(neko::Vec2f(2.0f, 3.0f), 4.0f),
			  neko::Circle(neko::Vec2f(4.0f, 5.0f), 4.0f),
			  neko::Circle(neko::Vec2f(6.0f, 7.0f), 4.0f),
			  neko::Circle(neko::Vec2f(11.0f, 13.0f), 4.0f) };
	
	array2 = { neko::Circle(neko::Vec2f(5.0f, 6.0f), 4.0f),
			   neko::Circle(neko::Vec2f(7.0f, 8.0f), 4.0f),
			   neko::Circle(neko::Vec2f(9.0f, 10.0f), 4.0f),
			   neko::Circle(neko::Vec2f(9.0f, 10.0f), 4.0f) };
	
	/*for(size_t i = 0; i < 4; i++)
	{
		array[i] = neko::Circle(pos, 4.0f);
		std::cout << pos << '\n';
		pos += neko::Vec2f(3.0f,3.0f);
	}
	for(size_t i = 0; i < 4; i++)
	{
		array[i] = neko::Circle(pos2, 4.0f);
		std::cout << pos2 << '\n';
		pos2 += neko::Vec2f(3.0f,3.0f);
	}*/

	neko::FourCircle n_circle(array);
	neko::FourCircle n_circle2(array2);
	auto result = n_circle.IntersectsIntrinsics(n_circle2);
	auto start = std::chrono::steady_clock::now();
	auto results = n_circle.IntersectsCircleIntrinsics(array2[2]);
	auto end = std::chrono::steady_clock::now();

	for (auto& tet : result)
	{
		std::cout << tet << "\n";
		EXPECT_TRUE(tet == array[tet].Intersects(array2[tet]));

	}
	//EXPECT_TRUE(result[1] == array[1].Intersects(array2[1]));
	//EXPECT_TRUE(result[2] == array[2].Intersects(array2[2]));
	//EXPECT_TRUE(result[3] == array[3].Intersects(array2[3]));

	for (auto& tet : results)
	{
		std::cout << tet << "\n";
		EXPECT_TRUE(tet == array[tet].Intersects(array2[2]));

	}
	auto diff = end - start;
	std::cout << std::chrono::duration <double, std::nano>(diff).count() << " ns\n";
	//EXPECT_TRUE(results[1] == array[1].Intersects(array2[2]));
	//EXPECT_TRUE(results[2] == array[2].Intersects(array2[2]));
	//EXPECT_TRUE(results[3] == array[3].Intersects(array2[2]));
	
}

/*TEST(Engine, TestNSphere)
{	
	std::array<neko::Sphere, 4> array;
	array.fill(neko::Sphere(neko::Vec3f(3, 4,3), 4));

	neko::FourNSphere n_circle(array);
	auto result = n_circle.Intersect(array);

	for (auto& tet : result) 
	{
		std::cout << tet << "\n";
		EXPECT_TRUE(tet == array[0].Intersects(array[0]));
	}
	
}*/
