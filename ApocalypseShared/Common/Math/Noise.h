#pragma once
class Noise
{
public:
	static int GetYLevel(int x, int z);
	static int GetBiomeTemperature(int x, int z);
	static void SetNoiseSettings(int Octaves,double Frequency, int yLevelStretch, int biomeStretch);
private:
	static inline int octaves = 8; // 1-16
	static inline double frequency = 256.0;
	static inline int YLevelStretch = 96;
	static inline int BiomeStretch = 4;
};
