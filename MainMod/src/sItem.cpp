#include "sItem.h"
#include "sBlock.h"
namespace Script {
	void Item::Register(int iname, const ItemProperties&& ip)
	{
		itemProperties[iname] = ip;
	}
	void Item::RegisterItems()
	{
		Register(Block::Air, { 0 });
		Register(Block::Grass, { 1 });
		Register(Block::Log, { 4 });
		Register(Block::Dirt, { 2 });
		Register(Block::Stone, { 3 });
		Register(Block::Glass, { 7 });
		Register(Block::DryGrass, { 12 });
		Register(Block::Iron, { 6 });
		Register(Block::Leaves, { 8 });
		Register(Block::Water, { 14 });
		Register(Block::DirtSlab, { 2 });
		Register(Block::DirtStairs, { 2 });
		Register(Block::DirtVerticalSlab, { 2 });
		Register(Block::LogCarpet, { 4 });
		Register(Block::Haybale, { 5 });

		Register(Drygrassblade, { 9 });
		Register(Stick, { 11 });
		Register(StonePickaxe, { 13 });
	}
}