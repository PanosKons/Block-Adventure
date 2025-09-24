#include <windows.h>
#include "ScriptingManager.h"
#include "Logger.h"
#include "EntityManagerServer.h"
#include "EventManager.h"
#include "Common/Math/Noise.h"
#include "Common/InputAction.h"
//#pragma comment(lib, "Winmm.lib")
//#pragma comment(lib, "Version.lib")
//#pragma comment(lib, "Bcrypt.lib")

#include "Common/Blocks/Block.h"
#include "World/WorldManagerServer.h"
#include "Common/Networking/Packet.h"
#include "NetworkingServer.h"
#include "Common/Script/WorldGenerationData.h"

HMODULE dll;

template<typename Ret, typename... Args>
Ret Call(const std::string& name, Args&&... args) {
	auto funcPtr = reinterpret_cast<Ret(*)(Args...)>(GetProcAddress(dll, name.c_str()));
	if (!funcPtr) {
		ERROR("Function " + name + " not found!");
		throw std::runtime_error("Function " + name + " not found!");
	}
	return funcPtr(std::forward<Args>(args)...);
}

void ScriptingManager::Load()
{
	std::string path = "MainMod.dll"; //There is post build event command
	dll = LoadLibraryA(path.c_str());
	if (!dll) {
		ERR("Failed to load dll!");
	}
	int res = Call<int>("Core_Initialize");
	INFO("Initialized dll with code ", res);
}
void ScriptingManager::Shutdown()
{
	FreeLibrary(dll);
}
void ScriptingManager::ReloadAssembly()
{
	Load();
}

struct CSBlock
{
	Vector3<int> Position;
	int BlockType;
};

static void GetBlock(Vector3<int> Position, CSBlock* csblock)
{
	Block block = WorldManager::BaseWorld->GetBlock(Position);
	csblock->Position = block.Position;
	csblock->BlockType = block.GetBlockId();
}
static void GetPlayerFacingBlock(uint64_t UUID, CSBlock* csblock)
{
	Block block = EntityManager::GetPlayer(UUID)->GetFacingBlock();
	if (block.IsValid())
	{
		csblock->Position = block.Position;
		csblock->BlockType = block.GetBlockId();
	}
	else
	{
		csblock->Position = { 0,0,0 };
		csblock->BlockType = -1;
	}
}
static void GetPlayerFacingEntity(uint64_t UUID, Entity* csentity)
{
	Entity* entity = EntityManager::GetPlayer(UUID)->GetFacingEntity();
	if (entity != nullptr)
	{
		csentity->entityType = entity->entityType;
		csentity->Position = entity->Position;
		csentity->Hitbox = entity->Hitbox;
		csentity->UUID = entity->UUID;
	}
	else
	{
		csentity->entityType = 0;
	}
}
static void GetPlayerBlockToPlace(uint64_t UUID, CSBlock* csblock)
{
	Block block = EntityManager::GetPlayer(UUID)->GetBlockToPlace();
	if (block.IsValid())
	{
		csblock->Position = block.Position;
		csblock->BlockType = block.GetBlockId();
	}
	else
	{
		csblock->Position = { 0,0,0 };
		csblock->BlockType = -1;
	}
}
static void ReplaceBlock(CSBlock* csblock, int BlockType)
{
	WorldManager::ReplaceBlock(WorldManager::BaseWorld->GetBlock(csblock->Position), BlockType);

	ReplaceBlockData data;
	data.blockType = BlockType;
	data.Position = csblock->Position;
	NetworkingServer::SendDataAllClients(Packet::ReplaceBlock, data);
}
static int GetFirstAvaiableSlot(std::array<ItemStack, InventorySize>& Inventory, ItemStack itemStack)
{
	for (unsigned int i = 0; i < Inventory.size(); i++)
	{
		if (Inventory[i].GetItemType() == itemStack.GetItemType()) return i;
	}
	for (unsigned int i = 0; i < Inventory.size(); i++)
	{
		if (Inventory[i].GetCount() == 0) return i;
	}
	return -1;
}
static void AddItemToInventory(uint64_t UUID, ItemStack item)
{
	PlayerInventoryData data;
	data.UUID = UUID;
	data.Inventory = EntityManager::GetPlayer(UUID)->PlayerInventory;
	ItemStack& stack = data.Inventory[GetFirstAvaiableSlot(data.Inventory, item)];
	stack = { item.GetItemType(),item.GetCount() + stack.GetCount() };
	EntityManager::GetPlayer(UUID)->PlayerInventory = data.Inventory;
	NetworkingServer::SendDataAllClients(Packet::PlayerInventory, data);
}
static void RemoveItemFromInventory(uint64_t UUID, ItemStack item)
{
	PlayerInventoryData data;
	data.UUID = UUID;
	data.Inventory = EntityManager::GetPlayer(UUID)->PlayerInventory;
	ItemStack& stack = data.Inventory[GetFirstAvaiableSlot(data.Inventory, item)];
	stack = { item.GetItemType(),stack.GetCount() - item.GetCount() };
	EntityManager::GetPlayer(UUID)->PlayerInventory = data.Inventory;
	NetworkingServer::SendDataAllClients(Packet::PlayerInventory, data);
}
static void GetHoldingItemStack(uint64_t UUID, ItemStack* item)
{
	ItemStack& itemStack = EntityManager::GetPlayer(UUID)->PlayerInventory[EntityManager::GetPlayer(UUID)->ActiveSlot];
	item->Count = itemStack.Count;
	item->ItemType = itemStack.ItemType;
}
static void SetPosition(uint64_t UUID, Vector3<double> Position)
{
	EntityManagerServer::GetPlayer(UUID)->Position = Position;

	PlayerPositionData data;
	data.playerPosition = Position;
	data.UUID = UUID;
	NetworkingServer::SendDataAllClients(Packet::PlayerPosition, data);
}
static void GetPosition(uint64_t UUID, Vector3<double>* Position)
{
	Vector3<double> position = EntityManagerServer::GetPlayer(UUID)->Position;
	Position->x = position.x;
	Position->y = position.y;
	Position->z = position.z;
}
static void IncrementRenderDistance(int value)
{
	WorldManagerServer::IncrementRenderDistance(value);
}
static void HandleGui(uint64_t UUID, Vector4<float> Color, Gui::Slot* Slots, int SlotCount, bool Open)
{
	GuiData data;
	data.gui.Color = Color;
	std::copy(Slots, Slots + SlotCount, data.gui.Slots.begin());
	data.Open = Open;
	NetworkingServer::SendDataToClient(UUID, Packet::HandleGui, data);
}

static void CreateEntity(Entity entity)
{
	EntityManager::CreateEntity(entity);
	EntityCreateData data;
	data.entity = entity;
	NetworkingServer::SendDataAllClients(Packet::CreateEntity, data);
}
static void KillEntity(uint64_t UUID)
{
	EntityManager::KillEntity(UUID);
	EntityKillData data;
	data.UUID = UUID;
	NetworkingServer::SendDataAllClients(Packet::KillEntity, data);
}

void ScriptingManager::OnGlobalUpdateEvent(double TimeStep)
{
	//if (GlobalUpdateMethod == nullptr) return;
	//mono_runtime_invoke(GlobalUpdateMethod, nullptr, nullptr, nullptr);
}
void ScriptingManager::OnPlayerUpdateEvent(uint64_t UUID)
{
	//if (PlayerUpdateMethod == nullptr) return;
	//void* UUIDp = &UUID;
	//mono_runtime_invoke(PlayerUpdateMethod, nullptr, &UUIDp, nullptr);
}
void ScriptingManager::OnCommandEvent(uint64_t UUID, Command& command)
{
	Call<void>("Core_OnCommand", UUID, command);
}
void ScriptingManager::OnActionEvent(uint64_t UUID, int identifier)
{
	//if (ActionEventMethod == nullptr) return;

	//std::array<void*, 2> parameters = {
	//	&UUID,
	//	(void*)&identifier
	//};
	//mono_runtime_invoke(ActionEventMethod, nullptr, parameters.data(), nullptr);
}

void ScriptingManager::GlobalUpdateEvent(double TimeStep)
{
	OnGlobalUpdateEvent(TimeStep);
	for (auto& [UUID, player] : EntityManager::Players)
	{
		OnPlayerUpdateEvent(UUID);
	}

	CommandEvent* commandEvent;
	while ((commandEvent = EventManager::GetCommandEvent()) != nullptr)
	{
		OnCommandEvent(commandEvent->UUID, commandEvent->command);
	}
	ActionEvent* actionEvent;
	while ((actionEvent = EventManager::GetActionEvent()) != nullptr)
	{
		OnActionEvent(actionEvent->UUID, actionEvent->identifier);
	}
}

void ScriptingManager::Start()
{
	Load();
	WorldGenerationData worldgendata = Call<WorldGenerationData>("Core_GetWorldGenerationData");
	Noise::SetNoiseSettings(worldgendata.Octaves, worldgendata.Frequency, worldgendata.YLevelStretch, worldgendata.BiomeStretch);

	Block::blockProperties = *Call<std::vector<BlockProperties>*>("Block_GetRegisteredBlocks");

	Block::FillerBlock = worldgendata.Filler;
	Block::UndergroundBlock = worldgendata.UnderGround;
	Block::DirtBlock = worldgendata.Dirt;
	Block::DryTopBlock = worldgendata.DryTop;
	Block::WetTopBlock = worldgendata.WetTop;
	Block::DeadTopBlock = worldgendata.DeadTop;
	Block::StoneTopBlock = worldgendata.StoneTop;
	Block::OreBlock = worldgendata.Ore;
	Block::WaterBlock = worldgendata.Water;

	Item::itemProperties = *Call<std::vector<ItemProperties>*>("Item_GetRegisteredItems");
	Block::blockModels = *Call<std::vector<Model>*>("Block_GetRegisteredModels");
	InputAction::inputActions = *Call<std::vector<InputAction>*>("sInputAction_GetInputActions");
}