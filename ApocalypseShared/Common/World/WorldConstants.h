#pragma once
enum class Context
{
	Server, Client
};
constexpr double TimeStep = 0.02;

constexpr int InventorySize = 9;

constexpr int ChunkSize = 32;
constexpr int ChunkVolume = ChunkSize * ChunkSize * ChunkSize;
constexpr int BIG_NUMBER = 0x10000000000000;