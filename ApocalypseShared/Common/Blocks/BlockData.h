#pragma once
enum class BLOCK_ID
{
	Invalid, Air, Cobblestone, Grass, Log, Iron, Dirt, Glass, Leaves, Water, DryGrass
};
struct BlockData
{
	unsigned short blockId;
	unsigned char RenderedSides = 64; // front,right,back,left,bottom,top,renders at all
};