#include "pch.h"
#include "Noise.h"
#include "../Engine/scr/vendor/PerlinNoise.hpp"
constexpr unsigned int seed = 12;
const siv::PerlinNoise perlin(seed);
int Noise::GetYLevel(int x, int z)
{
	double a = perlin.accumulatedOctaveNoise2D_0_1(x / frequency, z / frequency, octaves);
	return (int)(a * YLevelStretch);
}
int Noise::GetBiomeTemperature(int x, int z)
{
	double a = perlin.accumulatedOctaveNoise2D_0_1(x / frequency + 10000, z / frequency + 10000, octaves);
	return (int)(a * BiomeStretch);
}

void Noise::SetNoiseSettings(int Octaves, double Frequency, int YlevelStretch, int biomeStretch)
{
	octaves = Octaves;
	frequency = Frequency;
	YLevelStretch = YlevelStretch;
	BiomeStretch = biomeStretch;
}
