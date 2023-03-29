#include "pch.h"
#include "Noise.h"
#include "../Engine/scr/vendor/PerlinNoise.hpp"
constexpr unsigned int seed = 12;
const siv::PerlinNoise perlin(seed);
int Noise::GetYLevel(int x, int z)
{
	double a = perlin.accumulatedOctaveNoise2D_0_1(x / worldGenerationData.frequency, z / worldGenerationData.frequency, worldGenerationData.octaves);
	return (int)(a * worldGenerationData.YLevelStretch);
}
int Noise::GetBiomeTemperature(int x, int z)
{
	double a = perlin.accumulatedOctaveNoise2D_0_1(x / worldGenerationData.frequency + 10000, z / worldGenerationData.frequency + 10000, worldGenerationData.octaves);
	return (int)(a * worldGenerationData.BiomeStretch);
}

void Noise::SetNoiseSettings(WorldGenerationData& worldgendata)
{
	worldGenerationData = worldgendata;
}
