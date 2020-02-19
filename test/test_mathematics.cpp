//
// Created by efarhan on 11/16/19.
//
#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include <gtest/gtest.h>
#include <mathematics/func_table.h>
#include <mathematics/circle.h>
const float maxNmb = 100.0f;

void RandomFill(std::vector<float>& numbers, float start = -maxNmb, float end = maxNmb)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{start, end};
    std::generate(numbers.begin(), numbers.end(), [&g, &dist](){return dist(g);});
}

TEST(Engine, TestMathematics)
{
    neko::FuncTable<float> sinFuncTable(0.0f, M_PI, [](float x){ return sinf(x);});
    sinFuncTable.GenerateTable();
    size_t sampleSize = 1024;
    std::vector<float> localNumbers(sampleSize);
    RandomFill(localNumbers, 0.0f, M_PI);

    float error = 0.0f;
    for(auto v : localNumbers)
    {
        error += sinFuncTable.GetValue(v) - sinf(v);
    }
    error /= float(sampleSize);
    std::cout << "Error margin for sinFuncTable with resolution 512: "<<error<<"\n";
}

TEST(Engine, TestContact)
{
	
    neko::Circle2D circleA(neko::Vec2f(2,2), 2);
    neko::Circle2D circleB(neko::Vec2f(3, 3), 3);

    neko::Sphere3D sphere1(neko::Vec3f(2, 2, 2), 2);
    neko::Sphere3D sphere2(neko::Vec3f(3, 3, 2), 2);

    neko::Rect2f rect(neko::Vec2f(3, 3), neko::Vec2f(2, 2));
    neko::Circle2D circleC(neko::Vec2f(4, 3), 2);
	
    std::cout << "Circle vs circle : " << circleA.CirclesIntersect(circleB) << "\n";
    std::cout << "Sphere vs sphere : " << sphere1.CirclesIntersect(sphere2) << "\n";
    std::cout << "Circle vs rect : " << circleC.RectCircleIntersect(rect) << "\n";

    std::cout << "Sphere vs plan : " << neko::Sphere3D::IsPlanCircleContact(sphere1, neko::Vec3f(0, 1, 0), neko::Vec3f(1, 1, 1)) << "\n";
}