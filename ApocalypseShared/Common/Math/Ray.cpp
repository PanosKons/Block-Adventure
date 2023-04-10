#include "pch.h"
#include "Ray.h"
#include "EngineMath.h"

Ray::Ray(const Vector3<double>& position, float pitch, float yaw)
: StartPosition(position),CurrentPosition(position),Pitch(pitch),Yaw(yaw)
{}

void Ray::Step(double scale)
{
	LastPosition = CurrentPosition;
	Vector3<double> p = GetDirection();
	p /= p.Magnitude();
	p *= scale;
	CurrentPosition += p;
}

Vector3<double> Ray::GetDirection()
{
	return { cos(Math::Radians(Yaw)), tan(Math::Radians(Pitch)), sin(Math::Radians(Yaw)) };
}
