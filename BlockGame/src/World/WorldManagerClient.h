#pragma once
#include "pch.h"
#include "Common\World\WorldManager.h"
class WorldManagerClient :
    public WorldManager
{
public:
    static void DeleteOldChunks();
    static void RequestNewChunks();
    static void RefreshBorderChunks(World* world, Vector3<int> ChunkPosition);
};

