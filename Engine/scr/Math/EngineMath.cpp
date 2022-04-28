#include "EngineMath.h"
#include <math.h>
float Math::Max(float a, float b)
{
	return a < b ? b : a;
}
float Math::Min(float a, float b)
{
	return a > b ? b : a;
}
int Math::Max(int a, int b)
{
	return a < b ? b : a;
}
int Math::Min(int a, int b)
{
	return a > b ? b : a;
}
int Math::Floor(float a)
{
	return (int)floor(a);
}
int Math::Powi(int a, int b)
{
	int res = 1;
	for (int i = 0; i < b; i++)
	{
		res *= a;
	}
	return res;
}