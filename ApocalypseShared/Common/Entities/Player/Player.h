#pragma once
#include "pch.h"
#include "Common/Math/Vector.h"
#include "Common/Blocks/Block.h"
#include "Common/Entities/Inventory/ItemStack.h"
#include "Common/World/WorldConstants.h"
#include "Common/Entities/Entity.h"
#include "Common/Entities/Credentials.h"

constexpr int MaxGUISlots = 64;
struct Gui
{
	struct Slot
	{
		Vector2<float> Position;
		bool Active = false;
	};

	Vector4<float> Color;
	std::array<Slot, MaxGUISlots> Slots;
};
enum class CameraMode
{
	FirstPerson,ThirdPersonBack,ThirdPersonFront
};
typedef std::array<ItemStack, InventorySize> Inventory;
class Player
{
public:

	Player(Credentials& credentials);
	Player() {};
	~Player();

	Block GetFacingBlock();
	Block GetBlockToPlace();
	Entity* GetFacingEntity();
	Vector3<double> GetLookPosition();

	Vector3<double> Position;
	Vector3<double> Hitbox;
	Vector3<double> Velocity;
	float Yaw;
	float Pitch;
	float MaxHealth;
	float Health;
	float Speed;
	bool Grounded;

	Credentials credentials;

	char ActiveSlot;
	Inventory PlayerInventory;

	float BlockPlaceDelay = 0.3f;

	float JumpCooldown = 0.0f;
	bool Crouch = false;
	bool Godmode = true;
	float ReachDistance;
	CameraMode cameraMode;

	//Field should be used only by the server
	bool IsReadyToReceivePackets = false;
};
