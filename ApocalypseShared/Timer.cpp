#include "pch.h"
#include "Timer.h"
#include "Logger.h"
Timer::Timer(const std::string& id)
:id(id)
{
	start = std::chrono::high_resolution_clock::now();
}
Timer::~Timer()
{
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	INFO("Function: " , id , " took: " , duration.count() * 1000.0f , "ms\n");
}