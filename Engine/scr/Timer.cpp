#include "Timer.h"
Timer::Timer(int id)
:id(id)
{
	start = std::chrono::high_resolution_clock::now();
}
Timer::~Timer()
{
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	std::cout << "Timer " << id << " took: " << duration.count() * 1000.0f << "ms\n";
}