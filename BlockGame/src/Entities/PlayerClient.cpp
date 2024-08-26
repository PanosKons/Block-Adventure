#include "PlayerClient.h"
#include "Common/Math/Vector.h"
#include "Input.h"
#include "Common/Blocks/Block.h"
#include "Entities/EntityManagerClient.h"
#include "Common/Math/EngineMath.h"
#include "Common/World/WorldManager.h"
#include "Logger.h"
#include "Networking/NetworkingClient.h"
#include "Renderer.h"
#include "RendererClient.h"
#include "Common/InputAction.h"

PlayerClient::PlayerClient(Credentials& credentials)
	: Player(credentials)
{
	Input::SetCursorCallback([](double xpos,double ypos) {EntityManagerClient::GetPlayer().CursorMoved(xpos,ypos); });
	Input::SetKeyCallback([](int key, int actioncode, int action, int mods) {EntityManagerClient::GetPlayer().KeyPressed(key, action); });
	Input::SetCharCallback([](unsigned int key) { EntityManagerClient::GetPlayer().TextInput(key); });
	Input::SetMouseCallback([](int button, int action, int mods) { EntityManagerClient::GetPlayer().MousePressed(button, action); });
}
void PlayerClient::TextInput(int codepoint)
{
	if (currentScreen == Screen::ChatBox && codepoint <= 255)
	{
		chatbox += (char)codepoint;
	}
}
void PlayerClient::MousePressed(int mouse, int action)
{
	if (action == ButtonState::Click) {
		for (InputAction& inputAction : InputAction::inputActions)
		{
			if (inputAction.button + 3 != mouse) continue;
			ActionPerformedData data;
			data.identifier = inputAction.identifier;
			NetworkingClient::SendDataToServer(Packet::ActionPerformed, data);
			break;
		}
	}
}
void PlayerClient::KeyPressed(int key, int action)
{
	if (key >= Key::n1 && key <= Key::n9 && action == ButtonState::Click && currentScreen == Screen::Game)
	{
		ActiveSlot = key - 49;
		//Notify the server
		{
			NetworkingClient::SendDataToServer(Packet::SelectSlot, /*SelectSlotData*/ ActiveSlot);
		}
	}
	if (key == Key::Slash && currentScreen == Screen::Game)
	{
		currentScreen = Screen::ChatBox;
	}
	if (key == Key::UpArrow && currentScreen == Screen::ChatBox)
	{
		chatbox = lastCommand;
	}
	if (key == Key::Enter && currentScreen == Screen::ChatBox)
	{
		{
			CommandData data;
			strcpy_s(data.command.data(),data.command.size() - 1, chatbox.c_str());
			NetworkingClient::SendDataToServer(Packet::Command, data);
		}

		lastCommand = chatbox;
		chatbox = "";
		currentScreen = Screen::Game;
	}
	if (key == Key::EscapeKey && currentScreen != Screen::Game)
	{
		chatbox = "";
		currentScreen = Screen::Game;
	}
	if (key == Key::BackSpace && currentScreen == Screen::ChatBox)
	{
		if(!chatbox.empty())
			chatbox.pop_back();
	}
	if (action == ButtonState::Click) {
		for (InputAction& inputAction : InputAction::inputActions)
		{
			if (inputAction.button != key) continue;
			ActionPerformedData data;
			data.identifier = inputAction.identifier;
			NetworkingClient::SendDataToServer(Packet::ActionPerformed, data);
			break;
		}
	}
}
void PlayerClient::CursorMoved(double xpos, double ypos)
{
	if (currentScreen != Screen::Game) return;
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

	//Notify the server
	{
		PlayerRotationData data;
		data.playerRotation = { Pitch,Yaw };
		data.UUID = credentials.UUID;
		NetworkingClient::SendDataToServer(Packet::PlayerRotation,data);
	}
}
void PlayerClient::InputTick(double TimeStep)
{
	Renderer::HideCursor(currentScreen == Screen::Game);
	if (currentScreen == Screen::Game)
	{
		if (Input::GetKeyState(Key::C) == ButtonState::Click)
		{
			EntityManagerClient::GetPlayer().Fov = 30.0f;
		}
		else
		{
			EntityManagerClient::GetPlayer().Fov = 70.0f;
		}

		if (Input::GetKeyState(Key::K) == ButtonState::Click)
		{
			Godmode = !Godmode;
		}

		if (Input::GetKeyState(Key::F5) == ButtonState::Click)
		{
			if (cameraMode == CameraMode::FirstPerson)
				cameraMode = CameraMode::ThirdPersonBack;
			else if (cameraMode == CameraMode::ThirdPersonBack)
				cameraMode = CameraMode::ThirdPersonFront;
			else
				cameraMode = CameraMode::FirstPerson;
		}

		Crouch = false;
		if (Input::GetKeyState(Key::Shift) == ButtonState::Click || Input::GetKeyState(Key::Shift) == ButtonState::Hold && Grounded)
		{
			Speed = 2.0f;
			Crouch = true;
		}
		else if (Input::GetKeyState(Key::Control) == ButtonState::Click || Input::GetKeyState(Key::Control) == ButtonState::Hold)
		{
			Speed = 6.0f;
			if (Godmode) Speed = 50.0f;
		}
		else
		{
			Speed = 4.0f;
		}

		if (Input::GetKeyState(Key::W) == ButtonState::Hold || Input::GetKeyState(Key::W) == ButtonState::Click)
		{
			Velocity.x = Speed * cos(Math::Radians(Yaw));
			Velocity.z = Speed * sin(Math::Radians(Yaw));
		}
		else if (Input::GetKeyState(Key::S) == ButtonState::Hold || Input::GetKeyState(Key::S) == ButtonState::Click)
		{
			Velocity.x = -Speed * cos(Math::Radians(Yaw));
			Velocity.z = -Speed * sin(Math::Radians(Yaw));
		}
		else if (Input::GetKeyState(Key::D) == ButtonState::Hold || Input::GetKeyState(Key::D) == ButtonState::Click)
		{
			Velocity.x = Speed * cos(Math::Radians(Yaw + 90));
			Velocity.z = Speed * sin(Math::Radians(Yaw + 90));
		}
		else if (Input::GetKeyState(Key::A) == ButtonState::Hold || Input::GetKeyState(Key::A) == ButtonState::Click)
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
			if (Input::GetKeyState(Key::Space) == ButtonState::Click)
			{
				Velocity.y = Speed;
			}
			else if (Input::GetKeyState(Key::Shift) == ButtonState::Click)
			{
				Velocity.y = -Speed;
			}
			else
			{
				Velocity.y = 0.0;
			}
		}

		JumpCooldown -= (float)TimeStep;
		if (Input::GetKeyState(Key::Space) == ButtonState::Click || Input::GetKeyState(Key::Space) == ButtonState::Hold && Grounded && !Godmode && JumpCooldown <= 0)
		{
			Crouch = false;
			Velocity.y = 7.2f;
			JumpCooldown += 0.4f;
		}
	}

	if (Godmode == false) Velocity.y -= GravityConstant * TimeStep;

	//Check collisions
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
		if (EntityManagerClient::CheckCollision({ Position.x + Velocity.x * TimeStep, Position.y, Position.z }, Hitbox))
		{
			Velocity.x = 0;
		}
		if (EntityManagerClient::CheckCollision({ Position.x , Position.y + Velocity.y * TimeStep, Position.z }, Hitbox))
		{
			if (Velocity.y <= -16.0f && !Godmode) Health -= -(float)Velocity.y / 3.0f;
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
	//Check if stuck in a block
	{
		if (EntityManagerClient::CheckCollision({ Position.x, Position.y, Position.z }, Hitbox))
		{
			Velocity.y = 2.0;
		}
	}
	//Apply the velocity to the position
	Position += Velocity * TimeStep;

	if (currentScreen == Screen::GUI)
	{
		if (Input::GetMouseState(Mouse::Left) == ButtonState::Click)
		{
			Vector2<double> cursorPosition = Input::GetCursorPosition();
			for (int i = 0; i < activeGui.Slots.size(); i++)
			{
				if (activeGui.Slots[i].Active) {
					Vector2<float> Position = RendererClient::SlotToPixel(activeGui.Slots[i].Position);
					if (cursorPosition.x >= Position.x && cursorPosition.x <= Position.x + SlotsX - 2 &&
						cursorPosition.y >= Position.y && cursorPosition.y <= Position.y + SlotsX - 2)
					{
						if(selectedSlot == -1)
							selectedSlot = i;
						else
						{
							if (selectedSlot == i) selectedSlot = -1;
							else if (activeInventory[i].IsValid())
							{
								ItemStack stack = activeInventory[i];
								activeInventory[i] = activeInventory[selectedSlot];
								activeInventory[selectedSlot] = stack;
							}
							else
							{
								activeInventory[i] = activeInventory[selectedSlot];
								activeInventory[selectedSlot] = ItemStack();
								selectedSlot = -1;
							}
						}
						break;
					}
				}
			}
		}
	}
}