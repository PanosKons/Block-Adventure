#include "pch.h"
#include "EngineMath.h"
#include <math.h>
int Math::Floor(float a)
{
	return (int)floor(a);
}
int Math::Abs(int a)
{
	if (a >= 0)
		return a;
	else return -a;
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