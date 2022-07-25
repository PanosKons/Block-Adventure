#include <Engine.h>
#include "mat4.h"
mat4::mat4(
	float xa, float xb, float xc, float xd,
	float ya, float yb, float yc, float yd,
	float za, float zb, float zc, float zd,
	float wa, float wb, float wc, float wd
)

{
	data[0] = { xa,ya,za,wa };
	data[1] = { xb,yb,zb,wb };
	data[2] = { xc,yc,zc,wc };
	data[3] = { xd,yd,zd,wd };
}
mat4::mat4()
{
	data[0] = { 0,0,0,0 };
	data[1] = { 0,0,0,0 };
	data[2] = { 0,0,0,0 };
	data[3] = { 0,0,0,0 };
}