#pragma once
#include "Common/Blocks/Block.h"
#include "sBlock.h"
namespace Script {
	class Item
	{
	public:
		enum ItemNames
		{
			Invalid = -1, Drygrassblade = Block::BlockTypeSize, Stick, StonePickaxe, ItemTypeSize
		};
		static void Register(int iname, const ItemProperties&& ip);
		static void RegisterItems();
		static std::vector<ItemProperties>* GetRegisteredItems() {
			return &itemProperties;
		}
	private:
		inline static std::vector<ItemProperties> itemProperties = std::vector<ItemProperties>(ItemTypeSize);
	};
}

