#pragma once
#include "Vector.h"

class Ray {
public:
	Ray(const Vector3<double>& position, float pitch, float yaw);
	void Step(double scale);
	Vector3<double> GetDirection();
	inline double GetLengthSquared() const { return (CurrentPosition.x - StartPosition.x) * (CurrentPosition.x - StartPosition.x) + (CurrentPosition.y - StartPosition.y) * (CurrentPosition.y - StartPosition.y) + (CurrentPosition.z - StartPosition.z) * (CurrentPosition.z - StartPosition.z); };

	Vector3<double> StartPosition;
	Vector3<double> CurrentPosition;
	Vector3<double> LastPosition;
	float Yaw;
	float Pitch;
};
