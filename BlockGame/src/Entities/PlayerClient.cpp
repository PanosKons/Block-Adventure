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

PlayerClient::PlayerClient(Credentials& credentials)
	: Player(credentials)
{
	Input::SetCursorCallback([](double xpos,double ypos) {EntityManagerClient::GetPlayer().CursorMoved(xpos,ypos); });
	Input::SetKeyCallback([](int key, int actioncode, int action, int mods) {EntityManagerClient::GetPlayer().KeyPressed(key, action); });
	Input::SetCharCallback([](unsigned int key) { EntityManagerClient::GetPlayer().TextInput(key); });
}
void PlayerClient::TextInput(int codepoint)
{
	if (currentScreen == Screen::ChatBox && codepoint <= 255)
	{
		chatbox += (char)codepoint;
	}
}
void PlayerClient::KeyPressed(int key, int action)
{
	if (key >= Key::n1 && key <= Key::n9 && action == Action::Press && currentScreen == Screen::Game)
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
	if (key == Key::UpArrow)
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
	//Use C to zoom
	{
		if (Input::GetKeyState(Key::C) == Action::Press && currentScreen == Screen::Game)
		{
			EntityManagerClient::GetPlayer().Fov = 30.0f;
		}
		else
		{
			EntityManagerClient::GetPlayer().Fov = 70.0f;
		}
	}
	//Use P to reload assembly
	{
		static bool prev = false;
		if (Input::GetKeyState(Key::P) == Action::Press && currentScreen == Screen::Game && !prev)
		{
			prev = true;
			KeyData data;
			data.PKeyPressed = true;
			NetworkingClient::SendDataToServer(Packet::KeyPress, data);
		}
		else if(Input::GetKeyState(Key::P) == Action::Release) prev = false;
	}
	//Use E open test server gui // Bug: it doesnt close
	{
		static bool prev = false;
		if (Input::GetKeyState(Key::E) == Action::Press && currentScreen != Screen::ChatBox && !prev)
		{
			prev = true;
			KeyData data;
			data.EKeyPressed = true;
			NetworkingClient::SendDataToServer(Packet::KeyPress, data);
		}
		else if (Input::GetKeyState(Key::E) == Action::Release) prev = false;
	}
	//Use I to open player inventory
	{
		static bool prev = false;
		if (Input::GetKeyState(Key::I) == Action::Press && currentScreen != Screen::ChatBox && !prev)
		{
			prev = true;
			//Open gui
		}
		else if (Input::GetKeyState(Key::I) == Action::Release) prev = false;
	}
	//Use K to toggle editor mode
	{
		static Action lastState = Action::Release;
		if (Input::GetKeyState(Key::K) == Action::Press && currentScreen == Screen::Game)
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
	//Use F5 to toggle CameraMode
	{
		static Action lastState = Action::Release;
		if (Input::GetKeyState(Key::F5) == Action::Press && currentScreen == Screen::Game)
		{
			if (lastState == Action::Release)
			{
				if(cameraMode == CameraMode::FirstPerson)
					cameraMode = CameraMode::ThirdPersonBack;
				else if (cameraMode == CameraMode::ThirdPersonBack)
					cameraMode = CameraMode::ThirdPersonFront;
				else
					cameraMode = CameraMode::FirstPerson;
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
		if (Input::GetKeyState(Key::Shift) == Action::Press && currentScreen == Screen::Game && Grounded)
		{
			Speed = 2.0f;
			Crouch = true;
		}
		else if (Input::GetKeyState(Key::Control) == Action::Press && currentScreen == Screen::Game)
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
		if (Input::GetKeyState(Key::W) == Action::Press && currentScreen == Screen::Game)
		{
			Velocity.x = Speed * cos(Math::Radians(Yaw));
			Velocity.z = Speed * sin(Math::Radians(Yaw));
		}
		else if (Input::GetKeyState(Key::S) == Action::Press && currentScreen == Screen::Game)
		{
			Velocity.x = -Speed * cos(Math::Radians(Yaw));
			Velocity.z = -Speed * sin(Math::Radians(Yaw));
		}
		else if (Input::GetKeyState(Key::D) == Action::Press && currentScreen == Screen::Game)
		{
			Velocity.x = Speed * cos(Math::Radians(Yaw + 90));
			Velocity.z = Speed * sin(Math::Radians(Yaw + 90));
		}
		else if (Input::GetKeyState(Key::A) == Action::Press && currentScreen == Screen::Game)
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
			if (Input::GetKeyState(Key::Space) == Action::Press && currentScreen == Screen::Game)
			{
				Velocity.y = Speed;
			}
			else if (Input::GetKeyState(Key::Shift) == Action::Press && currentScreen == Screen::Game)
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
		if (Input::GetKeyState(Key::Space) == Action::Press && Grounded && !Godmode && currentScreen == Screen::Game && JumpCooldown <= 0)
		{
			Crouch = false;
			Velocity.y = 7.2f;
			JumpCooldown += 0.4f;
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
	//Check collisions
	{
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

	//Inventory
	if (currentScreen == Screen::GUI && Input::GetMouseState(Mouse::Left) == MouseState::Click)
	{
		Vector2<double> cursorPosition = Input::GetCursorPosition();
		for (int i = 0; i < activeGui.Slots.size(); i++)
		{
			if (activeGui.Slots[i].Active) {
				Vector2<float> Position = RendererClient::SlotToPixel(activeGui.Slots[i].Position);
				if (cursorPosition.x >= Position.x && cursorPosition.x <= Position.x + SlotsX - 2 &&
					cursorPosition.y >= Position.y && cursorPosition.y <= Position.y + SlotsX - 2)
				{
					INFO("Clicked Slot:", i);
					break;
				}
			}
		}
	}

	if (currentScreen == Screen::Game)
	{
		MouseStateData data;
		data.LeftMouse = Input::GetMouseState(Mouse::Left);
		data.RightMouse = Input::GetMouseState(Mouse::Right);
		data.MiddleMouse = Input::GetMouseState(Mouse::Middle);
		NetworkingClient::SendDataToServer(Packet::MouseState, data);
	}
}
