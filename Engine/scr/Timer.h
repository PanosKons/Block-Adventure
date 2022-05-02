#pragma once
#include <chrono>
#include <iostream>
class Timer
{
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;
public:
	int id;
	Timer(int id);
	~Timer();
};
