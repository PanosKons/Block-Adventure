#include "PlayerClient.h"
#include "Common/Math/Vector.h"
#include "Input.h"
#include "Common/Blocks/Block.h"
#include "Entities/EntityManagerClient.h"
#include "Common/Math/EngineMath.h"
#include "Common/World/WorldManager.h"
#include "Logger.h"

PlayerClient::PlayerClient()
{
	Input::SetCursorCallback([](double xpos,double ypos) {EntityManagerClient::GetPlayer().CursorMoved(xpos,ypos); });
}

void PlayerClient::CursorMoved(double xpos, double ypos)
{
	if (IsGUIOpen) return;
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}
	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;
	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	Yaw += xoffset;
	Pitch += yoffset;
	if (Yaw < -180)
		Yaw += 360;
	if (Yaw > 180)
		Yaw -= 360;
	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;
}

glm::vec3 PlayerClient::GetCameraFront()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	return glm::normalize(front);
}

glm::vec3 PlayerClient::GetCameraPosition()
{
	Vector3<double> view = GetLookPosition();
	return glm::vec3(view.x, view.y, view.z);
}

void PlayerClient::InputTick(double TimeStep)
{
	if (Input::GetKeyState(Key::W) == Action::Press && !IsGUIOpen)
	{
		Velocity.x = Speed * cos(Math::Radians(Yaw));
		Velocity.z = Speed * sin(Math::Radians(Yaw));
	}
	else if (Input::GetKeyState(Key::S) == Action::Press && !IsGUIOpen)
	{
		Velocity.x = -Speed * cos(Math::Radians(Yaw));
		Velocity.z = -Speed * sin(Math::Radians(Yaw));
	}
	else if (Input::GetKeyState(Key::D) == Action::Press && !IsGUIOpen)
	{
		Velocity.x = Speed * cos(Math::Radians(Yaw + 90));
		Velocity.z = Speed * sin(Math::Radians(Yaw + 90));
	}
	else if (Input::GetKeyState(Key::A) == Action::Press && !IsGUIOpen)
	{
		Velocity.x = -Speed * cos(Math::Radians(Yaw + 90));
		Velocity.z = -Speed * sin(Math::Radians(Yaw + 90));
	}
	else
	{
		Velocity.x = 0;
		Velocity.z = 0;
	}
	if (Input::GetKeyState(Key::Space) == Action::Press && !IsGUIOpen)
	{
		Velocity.y = Speed;
	}
	else if (Input::GetKeyState(Key::Shift) == Action::Press && !IsGUIOpen)
	{
		Velocity.y = -Speed;
	}
	else
	{
		Velocity.y = 0.0;
	}

	Position += Velocity * TimeStep;
	/*
	Block facingblock = GetFacingBlock();
	//DrawPlayer(facingblock);
	//Renderer::DrawGeometry(*m_VertexBuffer, *m_IndexBuffer);
	if (IsBreakingBlock)
	{
		if (facingblock.Position != BreakingBlockPosition || Input::GetMouseState(Mouse::Left) == Action::Release) IsBreakingBlock = false;
		if (Inventory[ActiveSlot].id == (int)ITEM_ID::Pickaxe && Inventory[ActiveSlot].type == TYPE::ITEM && WorldManager::BaseWorld->GetBlock(BreakingBlockPosition).GetBlockProperties().tool == TOOL::Pickaxe)
		{
			TimeToBreak -= (float)TimeStep * 60 * 12;
		}
		else
		{
			TimeToBreak -= (float)TimeStep * 60;
		}
		if (TimeToBreak < 0)
		{
			int index = GetFirstAvaiableSlot((int)WorldManager::BaseWorld->GetBlock(BreakingBlockPosition).GetBlockId(), TYPE::BLOCK);
			Inventory[index].id = (int)WorldManager::BaseWorld->GetBlock(BreakingBlockPosition).GetBlockId();
			Inventory[index].type = TYPE::BLOCK;
			Inventory[index].count++;
			WorldManager::BaseWorld->GetBlock(BreakingBlockPosition).OnBreak(BLOCK_ID::Air);
			IsBreakingBlock = false;
		}
	}
	else if (Input::GetMouseState(Mouse::Left) == Action::Press && !IsGUIOpen)
	{
		MarkBlockToBreak();
	}
	BlockPlaceDelay -= (float)TimeStep;
	if (Input::GetMouseState(Mouse::Right) == Action::Press && !IsGUIOpen && BlockPlaceDelay < 0)
	{
		Block block = GetBlockToPlace();
		if (block.data == nullptr) return;
		if (EntityManagerClient::CheckCollision(Position, Hitbox, block.Position) == true)
			return;
		if (Inventory[ActiveSlot].count > 0 && Inventory[ActiveSlot].type == TYPE::BLOCK)
		{
			block.OnBreak((BLOCK_ID)Inventory[ActiveSlot].id);
			Inventory[ActiveSlot].count--;
		}
		BlockPlaceDelay = 0.3f;
	}
	if (Input::GetKeyState(Key::G) == Action::Press && !IsGUIOpen)
	{
		Godmode = !Godmode;
	}

	if (Input::GetMouseState(Mouse::Middle) == Action::Press && !IsGUIOpen && Godmode)
	{
		ItemStack& stack = Inventory[GetFirstAvaiableSlot((int)facingblock.GetBlockId(), TYPE::BLOCK)];
		stack.id = (int)facingblock.GetBlockId();
		stack.count += 1;
		stack.type = TYPE::BLOCK;
	}

	JumpCooldown -= (float)TimeStep;

	if (!Godmode)
	{
		Velocity.y -= 24.0f * TimeStep;
		Grounded = EntityManagerClient::CheckCollision({ Position.x , Position.y + Velocity.y * TimeStep, Position.z }, Hitbox);
	}
	else
	{
		if (Velocity.y < 0) Velocity.y = 0;
	}
	Crouch = false;
	if (Input::GetKeyState(Key::Shift) == Action::Press && !IsGUIOpen)
	{
		Speed = 2.0f;
		Crouch = true;
	}
	else if (Input::GetKeyState(Key::Control) == Action::Press && !IsGUIOpen)
	{
		Speed = 6.0f;
		if (Godmode) Speed = 28.0f;
	}
	else
	{
		Speed = 4.0f;
	}

	if (Input::GetKeyState(Key::Space) == Action::Press && (Grounded || Godmode) && !IsGUIOpen && JumpCooldown <= 0)
	{
		Crouch = false;
		Velocity.y = 7.2f;
		JumpCooldown += 0.4f;
	}

	if (Crouch && Grounded)
	{
		if (!EntityManagerClient::CheckCollision({ Position.x + Velocity.x * TimeStep, Position.y + Velocity.y * TimeStep, Position.z }, Hitbox))
		{
			Velocity.x = 0;
		}
		if (!EntityManagerClient::CheckCollision({ Position.x , Position.y + Velocity.y * TimeStep, Position.z + Velocity.z * TimeStep }, Hitbox))
		{
			Velocity.z = 0;
		}
	}
	if (EntityManagerClient::CheckCollision({ Position.x + Velocity.x * TimeStep, Position.y, Position.z }, Hitbox))
	{
		Velocity.x = 0;
	}
	if (EntityManagerClient::CheckCollision({ Position.x , Position.y + Velocity.y * TimeStep, Position.z }, Hitbox))
	{
		if (Velocity.y <= -6.0f) Health -= -(float)Velocity.y / 3.0f;
		Velocity.y = 0;
	}
	if (EntityManagerClient::CheckCollision({ Position.x , Position.y, Position.z + Velocity.z * TimeStep }, Hitbox))
	{
		Velocity.z = 0;
	}
	*/
}
