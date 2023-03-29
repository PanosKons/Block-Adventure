#include "ScriptingManager.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "Logger.h"
#include "EntityManagerServer.h"
#include "EventManager.h"
#include "Common/Math/Noise.h"
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "Bcrypt.lib")

#if _DEBUG 1
#pragma comment(lib, "mono/lib/Debug/libmono-static-sgen.lib")
#else
#pragma comment(lib, "mono/lib/Release/libmono-static-sgen.lib")
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
	mono_set_assemblies_path("mono/lib");
    RootDomain = mono_jit_init("ScriptRuntime");
    ASSERT(RootDomain, "Failed to load mono");

    AppDomain = mono_domain_create_appdomain((char*)"GameDomain", nullptr);
	mono_domain_set(AppDomain, true);

    CoreAssembly = LoadCSharpAssembly("C:/CPP/Block-Adventure/ScriptCore/bin/Release/net7.0/ScriptCore.dll");
    //PrintAssemblyTypes(CoreAssembly);

    RegisterInternalCalls();

    EventClass = GetClassInAssembly(CoreAssembly, "Scripting", "Event");
    DataClass = GetClassInAssembly(CoreAssembly, "Scripting", "Data");
    // Allocate an instance of our class
    //MonoObject* classInstance = mono_object_new(AppDomain, eventClass);
    // Call the parameterless (default) constructor
    //mono_runtime_object_init(classInstance);
    
    //MonoMethod* update = mono_class_get_method_from_name(EventClass, "Update", 2);
    //double a = 0.2;
    //int b = 321;
    //void* params[2] = 
    //{
    //    &a,&b
    //};
    //mono_runtime_invoke(update, nullptr, params, nullptr);
    //MonoString* str = mono_string_new(AppDomain, "RE O ANGEL BROMA");
    //MonoMethod* print = mono_class_get_method_from_name(EventClass, "Print", 1);
    //void** par = (void**)&str;
    //mono_runtime_invoke(print, nullptr, par, nullptr);

    //uint64_t UUID = 123324;
    //void* UUIDp = &UUID;
    //mono_runtime_invoke(mono_class_get_method_from_name(EventClass, "OnLeftClick", 1), nullptr, &UUIDp, nullptr);
    //MonoClassField* field = mono_class_get_field_from_name(DataClass, "Blockss");

    //int value = 0;
    //mono_field_get_value(nullptr, field, &value);
    //mono_field_set_value(nullptr, field, &value);
    //INFO(value);

    mono_runtime_invoke(mono_class_get_method_from_name(EventClass, "Initialize", 0), nullptr, nullptr, nullptr);

    MonoMethod* GetWorldGenerationData = mono_class_get_method_from_name(DataClass, "GetWorldGenerationData", 0);
    WorldGenerationData worldgendata = *(WorldGenerationData*)mono_object_unbox(mono_runtime_invoke(GetWorldGenerationData, nullptr, nullptr, nullptr));
    Noise::SetNoiseSettings(worldgendata);

    MonoMethod* GetBlockCount = mono_class_get_method_from_name(DataClass, "GetBlockCount", 0);
    MonoObject* countObj = mono_runtime_invoke(GetBlockCount, nullptr, nullptr, nullptr);
    int BlockCount = *(int*)mono_object_unbox(countObj);
    MonoMethod* GetBlock = mono_class_get_method_from_name(DataClass, "GetBlock", 0);
    for (int i = 0; i < BlockCount; i++)
    {
        MonoObject* blocksObj = mono_runtime_invoke(GetBlock, nullptr, nullptr, nullptr);
        auto blockProperties = *(BlockProperties*)mono_object_unbox(blocksObj);
        Block::blockProperties.push_back(blockProperties);
    }
    Block::FillerBlock = 0;
    Block::UndergroundBlock = 3;
    Block::DirtBlock = 2;
    Block::DryTopBlock = 1;
    Block::WetTopBlock = 1;
    Block::DeadTopBlock = 2;
    Block::StoneTopBlock = 3;
    Block::OreBlock = 9;

    MonoMethod* GetItemCount = mono_class_get_method_from_name(DataClass, "GetItemCount", 0);
    MonoObject* icountObj = mono_runtime_invoke(GetItemCount, nullptr, nullptr, nullptr);
    int ItemCount = *(int*)mono_object_unbox(icountObj);
    MonoMethod* GetItem = mono_class_get_method_from_name(DataClass, "GetItem", 0);
    for (int i = 0; i < ItemCount; i++)
    {
        MonoObject* itemsObj = mono_runtime_invoke(GetItem, nullptr, nullptr, nullptr);
        Item::itemProperties.push_back(*(ItemProperties*)mono_object_unbox(itemsObj));
    }
}
void ScriptingManager::Shutdown()
{
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
    data.Inventory = EntityManager::GetPlayer(UUID)->Inventory;
    ItemStack& stack = data.Inventory[GetFirstAvaiableSlot(data.Inventory, item)];
    stack = { item.GetItemType(),item.GetCount() + stack.GetCount() };
    EntityManager::GetPlayer(UUID)->Inventory = data.Inventory;
    NetworkingServer::SendDataAllClients(Packet::PlayerInventory, data);
}
static void RemoveItemFromInventory(uint64_t UUID, ItemStack item)
{
    PlayerInventoryData data;
    data.UUID = UUID;
    data.Inventory = EntityManager::GetPlayer(UUID)->Inventory;
    ItemStack& stack = data.Inventory[GetFirstAvaiableSlot(data.Inventory, item)];
    stack = { item.GetItemType(),stack.GetCount() - item.GetCount() };
    EntityManager::GetPlayer(UUID)->Inventory = data.Inventory;
    NetworkingServer::SendDataAllClients(Packet::PlayerInventory, data);
}
static void SetPosition(uint64_t UUID, Vector3<double> Position)
{
    EntityManagerServer::GetPlayer(UUID)->Position = Position;

    PlayerPositionData data;
    data.playerPosition = Position;
    data.UUID = UUID;
    NetworkingServer::SendDataAllClients(Packet::PlayerPosition, data);
}

void ScriptingManager::RegisterInternalCalls()
{
    mono_add_internal_call("Scripting.Block::GetBlock", GetBlock);
    mono_add_internal_call("Scripting.Block::GetPlayerFacingBlock", GetPlayerFacingBlock);
    mono_add_internal_call("Scripting.Block::ReplaceBlock", ReplaceBlock);
    mono_add_internal_call("Scripting.Player::AddItemToInventory", AddItemToInventory);
    mono_add_internal_call("Scripting.Player::RemoveItemFromInventory", RemoveItemFromInventory);
    mono_add_internal_call("Scripting.Player::SetPosition", SetPosition);
}

void ScriptingManager::OnMouseEvent(uint64_t UUID, MouseState LeftMouse, MouseState RightMouse, MouseState MiddleMouse)
{
    static MonoMethod* LeftMouseEventMethod = mono_class_get_method_from_name(EventClass, "OnLeftClick", 1);
    static MonoMethod* RightMouseEventMethod = mono_class_get_method_from_name(EventClass, "OnRightClick", 1);
    static MonoMethod* MiddleMouseEventMethod = mono_class_get_method_from_name(EventClass, "OnMiddleClick", 1);

    void* UUIDp = &UUID;
    if (LeftMouse == MouseState::Click)
    {
        mono_runtime_invoke(LeftMouseEventMethod, nullptr, &UUIDp, nullptr);
    }
    if (RightMouse == MouseState::Click)
    {
        mono_runtime_invoke(RightMouseEventMethod, nullptr, &UUIDp, nullptr);
    }
    if (MiddleMouse == MouseState::Click)
    {
        mono_runtime_invoke(MiddleMouseEventMethod, nullptr, &UUIDp, nullptr);
    }
}

void ScriptingManager::OnCommandEvent(uint64_t UUID, Command& command)
{
    static MonoMethod* CommandEventMethod = mono_class_get_method_from_name(EventClass, "OnCommand", 2);
    MonoString* commandString = mono_string_new(AppDomain, command.data());

    std::array<void*, 2> parameters = {
        &UUID,
        commandString
    };

    mono_runtime_invoke(CommandEventMethod, nullptr, parameters.data(), nullptr);
}

void ScriptingManager::UpdateEvent()
{
    MouseEvent* mouseEvent;
    while ((mouseEvent = EventManager::GetMouseEvent()) != nullptr)
    {
        OnMouseEvent(mouseEvent->UUID, mouseEvent->LeftMouse, mouseEvent->RightMouse, mouseEvent->MiddleMouse);
    }
    CommandEvent* commandEvent;
    while ((commandEvent = EventManager::GetCommandEvent()) != nullptr)
    {
        OnCommandEvent(commandEvent->UUID, commandEvent->command);
    }
}
