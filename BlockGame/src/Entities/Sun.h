#pragma once
#include "Common/Math/Vector.h"
class Sun
{
public:
	//static void SetDayDuration(float seconds);
	static void Update();
	static Vector3<double> GetPosition();
	static Vector3<float> GetDirection();
private:
	inline static float angle = 1.0f;
};

