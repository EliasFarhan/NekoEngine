#include <gtest/gtest.h>
#include <engine/jobsystem.h>
#include <iostream>

namespace neko
{

int sum(int a, int b){
    return a+b;
}

TEST(Engine, TestJobSystem)
{
    Job<int(int,int)> job(sum, {5,3});
    auto result = job.Execute();
    std::cout << result << '\n';
}

}