#pragma once
#include "pch.h"
#include "Common/Math/Vector.h"
#include "Common/Blocks/Block.h"
#include "Common/Entities/Inventory/ItemStack.h"
#include "Common/World/WorldConstants.h"
#include "Common/Entities/Entity.h"
#include "Common/Entities/Credentials.h"

enum Mouse
{
	Left, Right, Middle
};
enum Key
{
	Space = 32,
	Slash = 47,
	n0 = 48, n1, n2, n3, n4, n5, n6, n7, n8, n9,
	A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	Shift = 340, Control, Alt,
	EscapeKey = 256, Enter, Tab, BackSpace,
	RightArrow = 262, LeftArrow, DownArrow, UpArrow,
	F1 = 290, F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12
};
enum MouseState
{
	Click, Hold, Idle
};
enum Action
{
	Release, Press, Repeat
};
constexpr int MaxGUISlots = 10;
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
	std::array<ItemStack, InventorySize> Inventory;

	float BlockPlaceDelay = 0.3f;

	float JumpCooldown = 0.0f;
	bool Crouch = false;
	bool Godmode = true;
	float ReachDistance;
	CameraMode cameraMode;

	//Field should be used only by the server
	bool IsReadyToReceivePackets = false;
};
