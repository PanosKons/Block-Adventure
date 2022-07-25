#include <Engine.h>
#include "Noise.h"
#include "../vendor/PerlinNoise.hpp"
constexpr double frequency = 8.0; //0.1 - 64.0
constexpr int octaves = 8; // 1-16
constexpr unsigned int seed = 12;
const siv::PerlinNoise perlin(seed);
int Noise::GetYLevel(int x, int z)
{
	double a = perlin.accumulatedOctaveNoise2D_0_1(x / 256.0f, z / 256.0f, octaves);
	return (int)(a * 96);
}
int Noise::GetBiomeTemperature(int x, int z)
{
	double a = perlin.accumulatedOctaveNoise2D_0_1(x / 256.0f + 1000, z / 256.0f + 1000, octaves);
	return (int)(a * 4);
}