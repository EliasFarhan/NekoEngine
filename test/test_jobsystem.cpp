#include <gtest/gtest.h>
#include <engine/jobsystem.h>

//#include <easy/profiler.h>

namespace neko
{

void Task(std::atomic<unsigned int>& currentDoneTasks)
{
    const auto TASK_WORK_TIME = std::chrono::seconds(1);

#ifdef USING_EASY_PROFILER
    EASY_BLOCK("JOBSYSTEM_DO_NOTHING");
#endif
    std::this_thread::sleep_for(TASK_WORK_TIME);
    ++currentDoneTasks;
}

TEST(Engine, TestJobSystem)
{
    const unsigned int TASKS_COUNT = 16;
    std::atomic<unsigned int> doneTasks = 0;

    {// JobSystem
#ifdef USING_EASY_PROFILER
        EASY_PROFILER_ENABLE;
        EASY_BLOCK("JOBSYSTEM_MAIN_THREAD")
    	{
#endif
		    JobSystem jobSystem;
		    for (size_t i = 0; i < TASKS_COUNT; ++i)
		    {
		        jobSystem.KickJob(std::function<void()>{[&doneTasks] { Task(doneTasks); }});
		    }
#ifdef USING_EASY_PROFILER
        }
    	EASY_END_BLOCK;
#endif
    }// !JobSystem

#ifdef USING_EASY_PROFILER
    profiler::dumpBlocksToFile("JobSystem.prof");
#endif

    // JobSystem must make main thread wait until all tasks are done before self-destructing.
    EXPECT_EQ(TASKS_COUNT, doneTasks);
}

}