#include "PlayerClient.h"
#include "Common/Math/Vector.h"
#include "Input.h"
#include "Common/Blocks/Block.h"
#include "Entities/EntityManagerClient.h"
#include "Common/Math/EngineMath.h"
#include "Common/World/WorldManager.h"
#include "Logger.h"
#include "Networking/NetworkingClient.h"
#include "UI/Commands.h"

PlayerClient::PlayerClient(Credentials& credentials)
	: Player(credentials)
{
	Input::SetCursorCallback([](double xpos,double ypos) {EntityManagerClient::GetPlayer().CursorMoved(xpos,ypos); });
	Input::SetKeyCallback([](int key, int actioncode, int action, int mods) {EntityManagerClient::GetPlayer().KeyPressed(key, action); });
	Input::SetCharCallback([](unsigned int key) { EntityManagerClient::GetPlayer().TextInput(key); });
}
void PlayerClient::TextInput(int codepoint)
{
	if (IsGUIOpen && codepoint <= 255)
	{
		chatbox += (char)codepoint;
	}
}
void PlayerClient::KeyPressed(int key, int action)
{
	if (key >= Key::n1 && key <= Key::n9 && action == Action::Press && !IsGUIOpen)
	{
		ActiveSlot = key - 49;
	}
	if (key == Key::Slash && !IsGUIOpen)
	{
		IsGUIOpen = true;
	}
	if (key == Key::Enter && IsGUIOpen)
	{
		Commands::ExecuteCommand(chatbox);
		chatbox = "";
		IsGUIOpen = false;
	}
	if (key == Key::EscapeKey && IsGUIOpen)
	{
		chatbox = "";
		IsGUIOpen = false;
	}
	if (key == Key::BackSpace && IsGUIOpen)
	{
		chatbox.pop_back();
	}
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
	//Use C to zoom
	{
		if (Input::GetKeyState(Key::C) == Action::Press && !IsGUIOpen)
		{
			EntityManagerClient::GetPlayer().Fov = 30.0f;
		}
		else
		{
			EntityManagerClient::GetPlayer().Fov = 70.0f;
		}
	}
	//Use K to toggle godmode
	{
		static Action lastState = Action::Release;
		if (Input::GetKeyState(Key::K) == Action::Press && !IsGUIOpen)
		{
			if (lastState == Action::Release)
			{
				Godmode = !Godmode;
				lastState = Action::Press;
			}
		}
		else
		{
			lastState = Action::Release;
		}
	}
	//Check whether to crouch or sprint
	{
		Crouch = false;
		if (Input::GetKeyState(Key::Shift) == Action::Press && !IsGUIOpen)
		{
			Speed = 2.0f;
			Crouch = true;
		}
		else if (Input::GetKeyState(Key::Control) == Action::Press && !IsGUIOpen)
		{
			Speed = 6.0f;
			if (Godmode) Speed = 100.0f;
		}
		else
		{
			Speed = 4.0f;
		}
	}
	//Apply movement
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
		if (Godmode == true)
		{
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
		}
		else
		{
			Velocity.y -= GravityConstant * TimeStep;
		}
	}
	//Jump mechanic
	{
		JumpCooldown -= (float)TimeStep;
		if (Input::GetKeyState(Key::Space) == Action::Press && Grounded && !Godmode && !IsGUIOpen && JumpCooldown <= 0)
		{
			Crouch = false;
			Velocity.y = 7.2f;
			JumpCooldown += 0.4f;
		}
	}
	//Check collisions
	{
		if (EntityManagerClient::CheckCollision({ Position.x + Velocity.x * TimeStep, Position.y, Position.z }, Hitbox))
		{
			Velocity.x = 0;
		}
		if (EntityManagerClient::CheckCollision({ Position.x , Position.y + Velocity.y * TimeStep, Position.z }, Hitbox))
		{
			if (Velocity.y <= -16.0f) Health -= -(float)Velocity.y / 3.0f;
			Velocity.y = 0;
			Grounded = true;
		}
		else
		{
			Grounded = false;
		}
		if (EntityManagerClient::CheckCollision({ Position.x , Position.y, Position.z + Velocity.z * TimeStep }, Hitbox))
		{
			Velocity.z = 0;
		}
	}
	//Not fall over the edge of a block
	{
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
	}
	//Apply the velocity to the position
	Position += Velocity * TimeStep;

	//Get the facing block
	Block facingblock = GetFacingBlock();
	//Break block functionality
	{
		if (IsBreakingBlock)
		{
			if (facingblock.Position != BreakingBlockPosition || Input::GetMouseState(Mouse::Left) == Action::Release) IsBreakingBlock = false;
			//if (Inventory[ActiveSlot].id == (int)ITEM_ID::Pickaxe && Inventory[ActiveSlot].type == TYPE::ITEM && WorldManager::BaseWorld->GetBlock(BreakingBlockPosition).GetBlockProperties().tool == TOOL::Pickaxe)
			//{
			//	TimeToBreak -= (float)TimeStep * 60 * 12;
			//}
			//else
			//{
			//	TimeToBreak -= (float)TimeStep * 60;
			//}
			TimeToBreak -= (float)TimeStep * 60;
			if (TimeToBreak < 0)
			{
				int index = GetFirstAvaiableSlot(ItemStack(ItemStackType::BlockItem,(ItemType)WorldManager::BaseWorld->GetBlock(BreakingBlockPosition).GetBlockId(),1));
				Inventory[index] = ItemStack(ItemStack(ItemStackType::BlockItem, (ItemType)WorldManager::BaseWorld->GetBlock(BreakingBlockPosition).GetBlockId(), Inventory[index].GetCount() + 1));
				WorldManager::ReplaceBlock(WorldManager::BaseWorld->GetBlock(BreakingBlockPosition),Block::FillerBlock);
				//Notify the server
				{
					Packet<SendReplaceBlock> packet;
					packet.InitMemory();
					packet.AddPacketData(PACKET_ID::ReplaceBlock);
					packet.AddPacketData<Vector3<int>>(BreakingBlockPosition);
					packet.AddPacketData(Block::FillerBlock);
					NetworkingClient::SendPacketToServer(packet);

				}
				IsBreakingBlock = false;
			}
		}
		else if (Input::GetMouseState(Mouse::Left) == Action::Press && !IsGUIOpen)
		{
			MarkBlockToBreak();
		}
	}
	//Place block functionality
	BlockPlaceDelay -= (float)TimeStep;
	if (Input::GetMouseState(Mouse::Right) == Action::Press && !IsGUIOpen && BlockPlaceDelay < 0)
	{
		Block block = GetBlockToPlace();
		if (!block.IsValid()) return;
		if (EntityManagerClient::CheckCollision(Position, Hitbox, block.Position) == true)
			return;
		if (Inventory[ActiveSlot].GetCount() > 0 && Inventory[ActiveSlot].GetItemStackType() == ItemStackType::BlockItem)
		{
			WorldManager::ReplaceBlock(block, (BlockType)Inventory[ActiveSlot].GetItemType());
			//Notify the server
			{
				Packet<SendReplaceBlock> packet;
				packet.InitMemory();
				packet.AddPacketData(PACKET_ID::ReplaceBlock);
				packet.AddPacketData<Vector3<int>>(block.Position);
				packet.AddPacketData(Inventory[ActiveSlot].GetItemType());
				NetworkingClient::SendPacketToServer(packet);

			}
			Inventory[ActiveSlot] = ItemStack(ItemStackType::BlockItem,Inventory[ActiveSlot].GetItemType(),Inventory[ActiveSlot].GetCount() - 1);
		}
		BlockPlaceDelay = 0.3f;
	}
	//Pick block functionality
	if (Input::GetMouseState(Mouse::Middle) == Action::Press && !IsGUIOpen && Godmode)
	{
		ItemStack& stack = Inventory[GetFirstAvaiableSlot((ItemStack(ItemStackType::BlockItem,(ItemType)facingblock.GetBlockId(),1)))];
		stack = ItemStack(ItemStackType::BlockItem, (ItemType)facingblock.GetBlockId(), stack.GetCount() + 1);
	}

}
