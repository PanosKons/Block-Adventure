#pragma once
#include <math.h>
template<typename T>
class Vector2
{
public:
	T x;
	T y;
	Vector2(T xx, T yy)
		:x(xx), y(yy)
	{}
	Vector2()
		: x(0), y(0)
	{}
	void operator+=(Vector2 other)
	{
		x += other.x;
		y += other.y;
	}
	Vector2 operator*(T value)
	{
		return { x * value,y * value };
	}
};
template<typename T>
class Vector3
{
public:
	T x;
	T y;
	T z;

	Vector3(T xx, T yy, T zz)
		:x(xx), y(yy), z(zz)
	{}
	Vector3(T xx, T yy)
		: x(xx), y(yy), z(0)
	{}
	Vector3(T xx)
		: x(xx), y(xx), z(xx)
	{}
	Vector3()
		: x(0), y(0), z(0)
	{}
	void operator+=(Vector2<T> other)
	{
		x += other.x;
		y += other.y;
	}
	void operator+=(Vector3<T> other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}
	Vector3 operator=(Vector2<T> other)
	{
		return { other.x,other.y,(T)0 };
	}
	void Magnitude()
	{
		return sqrt(powf(x, 2) + powf(y, 2) + powf(z, 2));
	}
	Vector3 operator*(T value)
	{
		return { x * value,y * value, z * value };
	}
};
template<typename T>
class Vector4
{
public:
	T x;
	T y;
	T z;
	T w;
	Vector4(T xx, T yy, T zz, T ww)
		:x(xx), y(yy), z(zz), w(ww)
	{}
	Vector4()
		: x(0), y(0), z(0), w(0)
	{}
	Vector4(T xx)
		: x(xx), y(xx), z(xx), w(xx)
	{}
};
namespace Vector
{
	inline Vector3<int> IntVector(Vector3<float>& other) { return Vector3<int>((int)other.x, (int)other.y, (int)other.z); };
	inline Vector3<float> FloatVector(Vector3<int>& other) { return Vector3<float>((float)other.x, (float)other.y, (float)other.z); };
};
