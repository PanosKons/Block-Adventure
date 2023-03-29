#pragma once
#include "pch.h"

constexpr int MaxCommandLength = 256;
typedef std::array<char, MaxCommandLength> Command;
enum class Context
{
	Server, Client
};
constexpr double IdealTimeStep = 0.02;
constexpr double GravityConstant = 24.0;
constexpr int InventorySize = 9;

constexpr int ChunkSize = 16;
constexpr int ChunkVolume = ChunkSize * ChunkSize * ChunkSize;