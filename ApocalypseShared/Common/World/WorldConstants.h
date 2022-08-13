#pragma once
enum class Context
{
	Server, Client
};
constexpr double IdealTimeStep = 0.02;

constexpr int InventorySize = 9;

constexpr int ChunkSize = 32;
constexpr int ChunkVolume = ChunkSize * ChunkSize * ChunkSize;
constexpr __int64 BIG_NUMBER = 0x10000000000000;