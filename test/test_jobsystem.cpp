#include <gtest/gtest.h>
#include <engine/jobsystem.h>
#ifdef EASY_PROFILE_USE
#include <easy/profiler.h>
#endif

namespace neko
{

void Task(std::atomic<unsigned int>& currentDoneTasks)
{
    const auto TASK_WORK_TIME = std::chrono::seconds(1);

#ifdef EASY_PROFILE_USE
    EASY_BLOCK("JOBSYSTEM_Task(std::atomic<unsigned int>&)");
#endif
    std::this_thread::sleep_for(TASK_WORK_TIME);
    ++currentDoneTasks;
}

TEST(Engine, TestJobSystem)
{
    const unsigned int TASKS_COUNT = 4;
    std::atomic<unsigned int> doneTasks = 0;

    {// JobSystem
#ifdef EASY_PROFILE_USE
        EASY_PROFILER_ENABLE;
        EASY_BLOCK("JOBSYSTEM_MAIN_THREAD")
    	{
#endif
		    JobSystem jobSystem;
		    for (size_t i = 0; i < TASKS_COUNT; ++i)
		    {
		        jobSystem.KickJob(std::function<void()>{[&doneTasks] { Task(doneTasks); }});
		    }
#ifdef EASY_PROFILE_USE
        }
    	EASY_END_BLOCK;
#endif
    }// !JobSystem

#ifdef EASY_PROFILE_USE
    profiler::dumpBlocksToFile("JobSystem.prof");
#endif

    // JobSystem must make main thread wait until all tasks are done before self-destructing.
    EXPECT_EQ(TASKS_COUNT, doneTasks);
}

}