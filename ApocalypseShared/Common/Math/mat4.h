#pragma once
#include "pch.h"
class mat4
{
public:
	mat4(
		float xa, float xb, float xc, float xd,
		float ya, float yb, float yc, float yd,
		float za, float zb, float zc, float zd,
		float wa, float wb, float wc, float wd
	);
	mat4();
	std::array<std::array<float, 4>, 4> data;
};
