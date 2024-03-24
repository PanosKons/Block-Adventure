#pragma once

class Math
{
public:
	template<typename T>
	static T Radians(T degrees)
	{
		return degrees * (T)(0.01745329251994329576923690768489);
	}
	template<typename T>
	static T Max(T a, T b)
	{
		return a < b ? b : a;
	}
	template<typename T>
	static T Min(T a, T b)
	{
		return a > b ? b : a;
	}
	static int Floor(float a);
	static int Floor(double a);
	static int Powi(int a, int b);
	static double Powd(double a, int b);
	static int Abs(int a);
	static double Abs(double a);
};
