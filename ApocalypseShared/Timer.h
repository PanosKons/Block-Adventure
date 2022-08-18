#pragma once
#include "pch.h"
class Timer
{
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;
public:
	std::string id;
	Timer(const std::string& id);
	~Timer();
};
