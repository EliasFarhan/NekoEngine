#include <gtest/gtest.h>
#include <engine/jobsystem.h>
#include <iostream>

namespace neko
{

void sum(int a, int b){
    std::cout << "The sum of " << a << '+' << b << " is " << a+b << ".\n";
}

TEST(Engine, TestJobSystem)
{
    JobSystem system;
    int a = 5;
    int b = 2;
    int c = 10;
    int d = 15;
    auto lambda = [a,b]{sum(a,b);};
    auto lambda2 = [c,d]{sum(c,d);};
    system.AddJob(lambda);
    system.AddJob(lambda2);
    system.ExecuteJobs();

    // auto lambda = [a,b]{sum(a,b);};
    // lambda();
}

}