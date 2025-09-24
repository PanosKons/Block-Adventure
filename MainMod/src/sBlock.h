#pragma once
#include "Common/Blocks/Block.h"
#include "sCore.h"

namespace Script {
	class Block
	{
	public:
		enum BlockNames { Invalid = -1, Air, Grass, Dirt, Stone, Glass, Leaves, Log, Water, DryGrass, Iron, DirtSlab, DirtStairs, DirtVerticalSlab, LogCarpet, Haybale, BlockTypeSize };
		enum BlockModelNames { SolidBlock, Slab, Stairs, VerticalSlab, Carpet, FakeBall, ModelTypeSize };
		static void Register(BlockNames bname, const BlockProperties&& bp);
		static void RegisterModel(BlockModelNames mname, const Model&& md);
		static void RegisterBlocks();
		static std::vector<BlockProperties>* GetRegisteredBlocks() {
			return &blockProperties;
		}
		static std::vector<Model>* GetRegisteredModels() {
			return &models;
		}
	private:
		inline static std::vector<BlockProperties> blockProperties = std::vector<BlockProperties>(BlockTypeSize);
		inline static std::vector<Model> models = std::vector<Model>(ModelTypeSize);
	};
}