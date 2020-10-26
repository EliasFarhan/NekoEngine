#include <gtest/gtest.h>
#include <engine/jobsystem.h>
#include <atomic>
#include <thread>
//#include <easy/profiler.h>

namespace neko
{

static void Task(std::atomic<unsigned int>& currentDoneTasks)
{
    const auto TASK_WORK_TIME = std::chrono::milliseconds(100);

#ifdef USING_EASY_PROFILER
    EASY_BLOCK("JOBSYSTEM_DO_NOTHING");
#endif
    std::this_thread::sleep_for(TASK_WORK_TIME);
    ++currentDoneTasks;
}

TEST(Engine, TestJobSystem)
{
    const size_t TASKS_COUNT = 16;
    std::atomic<unsigned int> doneTasks = 0;
    std::vector<std::unique_ptr<Job>> jobs(TASKS_COUNT);
    std::generate(jobs.begin(), jobs.end(),
            [&doneTasks]{ return std::make_unique<Job>([&doneTasks] { Task(doneTasks); });
    });

    JobSystem jobSystem;
    {// JobSystem
#ifdef USING_EASY_PROFILER
        EASY_PROFILER_ENABLE;
        EASY_BLOCK("JOBSYSTEM_MAIN_THREAD")
    	{
#endif
		    jobSystem.Init();
		    for (size_t i = 0; i < TASKS_COUNT; ++i)
		    {
                jobSystem.ScheduleJob(jobs[i].get(),JobThreadType::OTHER_THREAD);
		    }
		    jobSystem.Destroy();
#ifdef USING_EASY_PROFILER
        }
    	EASY_END_BLOCK;
#endif
    }// !JobSystem

#ifdef USING_EASY_PROFILER
    profiler::dumpBlocksToFile("JobSystem.prof");
#endif

    // JobSystem must make main thread wait until all tasks are done before self-destructing.
    //EXPECT_EQ(TASKS_COUNT, doneTasks);
}

}
