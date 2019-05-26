#pragma once
#include <string>
#include <thread>
#include <fstream>
#include <queue>
#include <condition_variable>



void initLog();
void logDebug(std::string msg);
void destroyLog();
