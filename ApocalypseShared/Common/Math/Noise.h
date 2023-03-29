#pragma once
struct WorldGenerationData
{
	int octaves = 8; // 1-16
	double frequency = 256.0;
	int YLevelStretch = 96;
	int BiomeStretch = 4;
};
class Noise
{
public:
	static int GetYLevel(int x, int z);
	static int GetBiomeTemperature(int x, int z);
	static void SetNoiseSettings(WorldGenerationData& worldgendata);
private:
	static inline WorldGenerationData worldGenerationData;
};
