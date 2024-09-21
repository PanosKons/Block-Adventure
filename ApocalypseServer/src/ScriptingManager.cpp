#include "ScriptingManager.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "Logger.h"
#include "EntityManagerServer.h"
#include "EventManager.h"
#include "Common/Math/Noise.h"
#include "Common/InputAction.h"
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "Bcrypt.lib")

#if _DEBUG 1
#pragma comment(lib, "mono/lib/debug-libmono-static-sgen.lib")
#else
#pragma comment(lib, "mono/lib/libmono-static-sgen.lib")
#endif

#include "Common/Blocks/Block.h"
#include "World/WorldManagerServer.h"
#include "Common/Networking/Packet.h"
#include "NetworkingServer.h"

static MonoDomain* RootDomain = nullptr;
static MonoDomain* AppDomain = nullptr;
static MonoAssembly* CoreAssembly = nullptr;

static MonoClass* EventClass = nullptr;
static MonoClass* DataClass = nullptr;

static MonoMethod* GlobalUpdateMethod = nullptr;
static MonoMethod* ActionEventMethod = nullptr;
static MonoMethod* CommandEventMethod = nullptr;
static MonoMethod* PlayerUpdateMethod = nullptr;


void PrintAssemblyTypes(MonoAssembly* assembly)
{
    MonoImage* image = mono_assembly_get_image(assembly);
    const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
    int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

    for (int32_t i = 0; i < numTypes; i++)
    {
        uint32_t cols[MONO_TYPEDEF_SIZE];
        mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

        const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
        const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

        printf("%s.%s\n", nameSpace, name);
    }
}
char* ReadBytes(const std::string& filepath, uint32_t* outSize)
{
    std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

    if (!stream)
    {
        // Failed to open the file
        return nullptr;
    }

    std::streampos end = stream.tellg();
    stream.seekg(0, std::ios::beg);
    uint32_t size = end - stream.tellg();

    if (size == 0)
    {
        // File is empty
        return nullptr;
    }

    char* buffer = new char[size];
    stream.read((char*)buffer, size);
    stream.close();

    *outSize = size;
    return buffer;
}
MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
{
    uint32_t fileSize = 0;
    char* fileData = ReadBytes(assemblyPath, &fileSize);

    // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
    MonoImageOpenStatus status;
    MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

    if (status != MONO_IMAGE_OK)
    {
        const char* errorMessage = mono_image_strerror(status);
        // Log some error message using the errorMessage data
        return nullptr;
    }

    MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
    mono_image_close(image);

    // Don't forget to free the file data
    delete[] fileData;

    return assembly;
}
MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
{
    MonoImage* image = mono_assembly_get_image(assembly);
    MonoClass* klass = mono_class_from_name(image, namespaceName, className);
    ASSERT(klass, "Unknown class");
    return klass;
}

void ScriptingManager::Load()
{
    AppDomain = mono_domain_create_appdomain((char*)"GameDomain", nullptr);
	mono_domain_set(AppDomain, true);

    WARN("Absolute file path");
    CoreAssembly = LoadCSharpAssembly("C:/Users/pkons/Desktop/Panos/Projects/C++ Projects/Block-Adventure/ScriptCore/bin/Release/net7.0/ScriptCore.dll");

    RegisterInternalCalls();

    EventClass = GetClassInAssembly(CoreAssembly, "Scripting", "Event");
    DataClass = GetClassInAssembly(CoreAssembly, "Scripting", "Data");

    GlobalUpdateMethod = mono_class_get_method_from_name(EventClass, "GlobalUpdateEvent", 0);
    PlayerUpdateMethod = mono_class_get_method_from_name(EventClass, "PlayerUpdateEvent", 1);

    CommandEventMethod = mono_class_get_method_from_name(EventClass, "OnCommand", 2);
    ActionEventMethod = mono_class_get_method_from_name(EventClass, "OnAction", 2);

    // Allocate an instance of our class
    //MonoObject* classInstance = mono_object_new(AppDomain, eventClass);
    // Call the parameterless (default) constructor
    //mono_runtime_object_init(classInstance);
    mono_runtime_invoke(mono_class_get_method_from_name(EventClass, "Initialize", 0), nullptr, nullptr, nullptr);
}
void ScriptingManager::Shutdown()
{
    mono_domain_set(mono_get_root_domain(), false);
    mono_domain_unload(AppDomain);
}


struct WorldGenerationData
{
    int Octaves;
    double Frequency;
    int YLevelStretch;
    int BiomeStretch;
    int Filler;
    int UnderGround;
    int Dirt;
    int DryTop;
    int WetTop;
    int DeadTop;
    int StoneTop;
    int Ore;
    int Water;
};
void ScriptingManager::ReloadAssembly()
{
    mono_domain_set(mono_get_root_domain(), false);
    mono_domain_unload(AppDomain);

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
static void HandleGui(uint64_t UUID,Vector4<float> Color,Gui::Slot* Slots,int SlotCount, bool Open)
{
    GuiData data;
    data.gui.Color = Color;
    std::copy(Slots, Slots + SlotCount, data.gui.Slots.begin());
    data.Open = Open;
    NetworkingServer::SendDataToClient(UUID,Packet::HandleGui, data);
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

void ScriptingManager::RegisterInternalCalls()
{
    mono_add_internal_call("Scripting.Block::GetBlock", GetBlock);
    mono_add_internal_call("Scripting.Block::GetPlayerFacingBlock", GetPlayerFacingBlock);
    mono_add_internal_call("Scripting.Block::GetPlayerBlockToPlace", GetPlayerBlockToPlace);
    mono_add_internal_call("Scripting.Block::ReplaceBlock", ReplaceBlock);

    mono_add_internal_call("Scripting.Player::AddItemToInventory", AddItemToInventory);
    mono_add_internal_call("Scripting.Player::RemoveItemFromInventory", RemoveItemFromInventory);
    mono_add_internal_call("Scripting.Player::SetPosition", SetPosition);
    mono_add_internal_call("Scripting.Player::GetPosition", GetPosition);
    mono_add_internal_call("Scripting.Player::GetHoldingItemStack", GetHoldingItemStack);
    mono_add_internal_call("Scripting.Player::IncrementRenderDistance", IncrementRenderDistance);
    mono_add_internal_call("Scripting.Player::HandleGui", HandleGui);

    mono_add_internal_call("Scripting.Entity::Create", CreateEntity);
    mono_add_internal_call("Scripting.Entity::GetPlayerFacingEntity", GetPlayerFacingEntity);
    mono_add_internal_call("Scripting.Entity::Kill", KillEntity);
}

void ScriptingManager::OnGlobalUpdateEvent(double TimeStep)
{
    if (GlobalUpdateMethod == nullptr) return;
    mono_runtime_invoke(GlobalUpdateMethod, nullptr, nullptr, nullptr);
}
void ScriptingManager::OnPlayerUpdateEvent(uint64_t UUID)
{
    if (PlayerUpdateMethod == nullptr) return;
    void* UUIDp = &UUID;
    mono_runtime_invoke(PlayerUpdateMethod, nullptr, &UUIDp, nullptr);
}
void ScriptingManager::OnCommandEvent(uint64_t UUID, Command& command)
{
    if (CommandEventMethod == nullptr) return;

    MonoString* commandString = mono_string_new(AppDomain, command.data());
    std::array<void*, 2> parameters = {
        &UUID,
        commandString
    };
    mono_runtime_invoke(CommandEventMethod, nullptr, parameters.data(), nullptr);
}
void ScriptingManager::OnActionEvent(uint64_t UUID, int identifier)
{
    if (ActionEventMethod == nullptr) return;

    std::array<void*, 2> parameters = {
        &UUID,
        (void*)&identifier
    };
    mono_runtime_invoke(ActionEventMethod, nullptr, parameters.data(), nullptr);
}

void ScriptingManager::GlobalUpdateEvent(double TimeStep)
{
    OnGlobalUpdateEvent(TimeStep);
    for (auto&[UUID, player] : EntityManager::Players)
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
    mono_set_assemblies_path("mono/lib");
    RootDomain = mono_jit_init("ScriptRuntime");
    ASSERT(RootDomain, "Failed to load mono");

    Load();

    MonoMethod* GetWorldGenerationData = mono_class_get_method_from_name(DataClass, "GetWorldGenerationData", 0);
    WorldGenerationData worldgendata = *(WorldGenerationData*)mono_object_unbox(mono_runtime_invoke(GetWorldGenerationData, nullptr, nullptr, nullptr));
    Noise::SetNoiseSettings(worldgendata.Octaves, worldgendata.Frequency, worldgendata.YLevelStretch, worldgendata.BiomeStretch);

    MonoMethod* GetBlockCount = mono_class_get_method_from_name(DataClass, "GetBlockCount", 0);
    MonoObject* countObj = mono_runtime_invoke(GetBlockCount, nullptr, nullptr, nullptr);
    int BlockCount = *(int*)mono_object_unbox(countObj);
    MonoMethod* GetBlocks = mono_class_get_method_from_name(DataClass, "GetBlocks", 0);
    MonoObject* blocksObj = mono_runtime_invoke(GetBlocks, nullptr, nullptr, nullptr);
    auto blockProperties = *(BlockProperties**)mono_object_unbox(blocksObj);
    Block::blockProperties = std::vector(blockProperties, blockProperties + BlockCount);

    Block::FillerBlock = worldgendata.Filler;
    Block::UndergroundBlock = worldgendata.UnderGround;
    Block::DirtBlock = worldgendata.Dirt;
    Block::DryTopBlock = worldgendata.DryTop;
    Block::WetTopBlock = worldgendata.WetTop;
    Block::DeadTopBlock = worldgendata.DeadTop;
    Block::StoneTopBlock = worldgendata.StoneTop;
    Block::OreBlock = worldgendata.Ore;
    Block::WaterBlock = worldgendata.Water;

    MonoMethod* GetItemCount = mono_class_get_method_from_name(DataClass, "GetItemCount", 0);
    MonoObject* icountObj = mono_runtime_invoke(GetItemCount, nullptr, nullptr, nullptr);
    int ItemCount = *(int*)mono_object_unbox(icountObj);
    MonoMethod* GetItems = mono_class_get_method_from_name(DataClass, "GetItems", 0);
    MonoObject* itemsObj = mono_runtime_invoke(GetItems, nullptr, nullptr, nullptr);
    auto itemProperties = *(ItemProperties**)mono_object_unbox(itemsObj);
    Item::itemProperties = std::vector(itemProperties, itemProperties + ItemCount);

    MonoMethod* GetModelCount = mono_class_get_method_from_name(DataClass, "GetModelCount", 0);
    MonoObject* mcountObj = mono_runtime_invoke(GetModelCount, nullptr, nullptr, nullptr);
    int ModelCount = *(int*)mono_object_unbox(mcountObj);
    MonoMethod* GetModel = mono_class_get_method_from_name(DataClass, "GetModel", 1);
    MonoMethod* GetFaceCount = mono_class_get_method_from_name(DataClass, "GetFaceCount", 1);
    for (size_t i = 0; i < ModelCount; i++)
    {
        void* p = &i;
        MonoObject* faceObj = mono_runtime_invoke(GetFaceCount, nullptr, &p, nullptr);
        int FaceCount = *(int*)mono_object_unbox(faceObj);

        MonoObject* modelObj = mono_runtime_invoke(GetModel, nullptr, &p, nullptr);
        auto faces = *(Face**)mono_object_unbox(modelObj);
        Block::blockModels.emplace_back();
        std::copy(faces, faces + FaceCount, Block::blockModels[i].Faces.begin());
    }
    MonoMethod* GetInputActionCount = mono_class_get_method_from_name(DataClass, "GetInputActionCount", 0);
    MonoObject* acountObj = mono_runtime_invoke(GetInputActionCount, nullptr, nullptr, nullptr);
    int InputActionCount = *(int*)mono_object_unbox(acountObj);
    MonoMethod* GetInputActions = mono_class_get_method_from_name(DataClass, "GetInputActions", 0);
    MonoObject* actionsObj = mono_runtime_invoke(GetInputActions, nullptr, nullptr, nullptr);
    auto inputActions = *(InputAction**)mono_object_unbox(actionsObj);
    InputAction::inputActions = std::vector(inputActions, inputActions + InputActionCount);
}