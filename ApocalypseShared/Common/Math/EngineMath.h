#pragma once

class Math
{
public:
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
	static int Powi(int a, int b);
	static int Abs(int a);
};
