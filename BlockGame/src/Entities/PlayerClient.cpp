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
		//Notify the server
		{
			NetworkingClient::SendDataToServer(Packet::SelectSlot, /*SelectSlotData*/ ActiveSlot);
		}
	}
	if (key == Key::Slash && !IsGUIOpen)
	{
		IsGUIOpen = true;
	}
	if (key == Key::UpArrow)
	{
		chatbox = lastCommand;
	}
	if (key == Key::Enter && IsGUIOpen)
	{
		{
			CommandData data;
			strcpy_s(data.command.data(),data.command.size() - 1, chatbox.c_str());
			NetworkingClient::SendDataToServer(Packet::Command, data);
		}
		lastCommand = chatbox;
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
	Renderer::HideCursor(!IsGUIOpen);
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
	//Use P to reload assembly
	{
		static bool prev = false;
		if (Input::GetKeyState(Key::P) == Action::Press && !IsGUIOpen && !prev)
		{
			prev = true;
			KeyData data;
			data.PKeyPressed = true;
			NetworkingClient::SendDataToServer(Packet::KeyPress, data);
		}
		else if(Input::GetKeyState(Key::P) == Action::Release) prev = false;
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
	//Use F5 to toggle CameraMode
	{
		static Action lastState = Action::Release;
		if (Input::GetKeyState(Key::F5) == Action::Press && !IsGUIOpen)
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
		if (Input::GetKeyState(Key::Shift) == Action::Press && !IsGUIOpen && Grounded)
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
	//Apply the velocity to the position
	Position += Velocity * TimeStep;
	if (!IsGUIOpen)
	{
		MouseStateData data;
		data.LeftMouse = Input::GetMouseState(Mouse::Left);
		data.RightMouse = Input::GetMouseState(Mouse::Right);
		data.MiddleMouse = Input::GetMouseState(Mouse::Middle);
		NetworkingClient::SendDataToServer(Packet::MouseState, data);
	}
}
