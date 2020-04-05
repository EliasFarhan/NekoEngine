#include <gtest/gtest.h>

#include <engine/jobsystem.h>
#ifdef NEKO_PROFILE
#include <easy/profiler.h>
#endif // !NEKO_PROFILE

namespace neko
{

void Task(std::atomic<unsigned int>& currentDoneTasks)
{
    const auto TASK_WORK_TIME = std::chrono::seconds(1);

#ifdef NEKO_PROFILE
    EASY_BLOCK("JOBSYSTEM_DO_NOTHING");
#endif// !NEKO_PROFILE
    std::this_thread::sleep_for(TASK_WORK_TIME);
    ++currentDoneTasks;
}

TEST(Engine, TestJobSystem)
{
    const unsigned int TASKS_COUNT = 16;
    std::atomic<unsigned int> doneTasks = 0;

    {// JobSystem
#ifdef NEKO_PROFILE
        EASY_PROFILER_ENABLE;
        EASY_BLOCK("JOBSYSTEM_MAIN_THREAD")
    	{
#endif// !NEKO_PROFILE
		    JobSystem jobSystem;
		    for (size_t i = 0; i < TASKS_COUNT; ++i)
		    {
		        jobSystem.KickJob(std::function<void()>{[&doneTasks] { Task(doneTasks); }});
		    }
#ifdef NEKO_PROFILE
        }
    	EASY_END_BLOCK;
#endif// !NEKO_PROFILE
    }// !JobSystem

#ifdef NEKO_PROFILE
    profiler::dumpBlocksToFile("JobSystem.prof");
#endif// !NEKO_PROFILE

    // JobSystem must make main thread wait until all tasks are done before self-destructing.
    EXPECT_EQ(TASKS_COUNT, doneTasks);
}

}// !neko
