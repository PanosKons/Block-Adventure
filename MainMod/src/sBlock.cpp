#include "sBlock.h"

namespace Script {
	void Block::Register(BlockNames bname, const BlockProperties&& bp)
	{
		blockProperties[bname] = bp;
	}
	void Block::RegisterModel(BlockModelNames mname, const Model&& md)
	{
		models[mname] = md;
	}
	std::array<unsigned char, 6> SimpleBlockTexture(unsigned char textId) {
		return { textId, textId, textId , textId , textId ,textId };
	}
	void Block::RegisterBlocks()
	{
		Register(Air, BlockProperties{ .textureSides = SimpleBlockTexture(0), .model = false, .translucency = 1.0f, .render = false});
		Register(Grass, BlockProperties{ .textureSides = {1, 1, 1, 1, 0, 2}, .model = false, .translucency = 1.0f, .render = true });
		Register(Log, BlockProperties{ .textureSides = {4, 4, 4, 4, 5, 5}, .model = false, .translucency = 1.0f, .render = true });
		Register(Dirt, BlockProperties{ .textureSides = SimpleBlockTexture(2), .model = false, .translucency = 1.0f, .render = true });
		Register(Stone, BlockProperties{ .textureSides = SimpleBlockTexture(3), .model = false, .translucency = 1.0f, .render = true });
		Register(Glass, BlockProperties{ .textureSides = SimpleBlockTexture(7), .model = true, .translucency = 1.0f, .render = true });
		Register(DryGrass, BlockProperties{ .textureSides = {12, 12, 12, 12, 10, 2}, .model = false, .translucency = 1.0f, .render = true });
		Register(Iron, BlockProperties{ .textureSides = SimpleBlockTexture(6), .model = false, .translucency = 1.0f, .render = true });
		Register(Leaves, BlockProperties{ .textureSides = SimpleBlockTexture(8), .model = true, .translucency = 1.0f, .render = true });
		Register(Water, BlockProperties{ .textureSides = SimpleBlockTexture(14), .model = true, .translucency = 0.4f, .render = true });
		//Register(DirtSlab, BlockProperties{ .textureSides = SimpleBlockTexture(), .model = false, .translucency = 1.0f, .render = true });
		//Add the rest of the blocks


		RegisterModel(SolidBlock, { .Faces = {
			Face{.position = {1.0f, 0.0f, 0.0f}, .size = {1.0f, 1.0f}, .direction = Direction::Right, .condition = 1, .textureIndex = 0, .Active = true},
			Face{.position = {0.0f, 0.0f, 0.0f}, .size = {1.0f, 1.0f}, .direction = Direction::Left, .condition = 2, .textureIndex = 1, .Active = true},
			Face{.position = {0.0f, 0.0f, 1.0f}, .size = {1.0f, 1.0f}, .direction = Direction::Forward, .condition = 4, .textureIndex = 2, .Active = true},
			Face{.position = {0.0f, 0.0f, 0.0f}, .size = {1.0f, 1.0f}, .direction = Direction::Backward, .condition = 8, .textureIndex = 3, .Active = true},
			Face{.position = {0.0f, 1.0f, 0.0f}, .size = {1.0f, 1.0f}, .direction = Direction::Up, .condition = 16, .textureIndex = 4, .Active = true},
			Face{.position = {0.0f, 0.0f, 0.0f}, .size = {1.0f, 1.0f}, .direction = Direction::Down, .condition = 32, .textureIndex = 4, .Active = true},
			} });
		//Add the rest of the models
	}
}