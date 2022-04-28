#include "ManagerUI.h"
#include "Renderer.h"
#include "GameManager.h"
#include <sstream>
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "GlobalVariables.h"
#include "Input.h"
#include <iostream>
#include "Commands.h"
static std::unique_ptr<IndexBuffer> m_IndexBuffer;
static std::unique_ptr<VertexBuffer> m_VertexBuffer;
static bool debugActive = false;
static bool TypingActive = false;
static std::string chatbox;
constexpr float charWidthOffset = 28.0f;
constexpr float charHeightOffset = 32.0f;
constexpr float charWidth = 32.0f;
constexpr float charHeight = 32.0f;

void ManagerUI::PrintString(std::string&& Text, Vector2<float> position)
{
	Vertex a;
	a.color = { 1,1,1,1 };
	a.texId = 13;
	a.position = { position.x,position.y,1.0f };
	for (char digit : Text)
	{
		int x = digit % 16;
		int y = digit / 16;
		y++;
		a.texCords = { x / 16.0f,1 - (y / 16.0f) };
		m_VertexBuffer->Add(a);
		a.position.y += charHeight;
		a.texCords.y += 1 / 16.0f;
		m_VertexBuffer->Add(a);
		a.position.x += charWidth;
		a.texCords.x += 1 / 16.0f;
		m_VertexBuffer->Add(a);
		a.position.y -= charHeight;
		a.texCords.y -= 1 / 16.0f;
		m_VertexBuffer->Add(a);
		m_IndexBuffer->AddRectangle();
		a.position.x -= charWidth - charWidthOffset;
	}
}
void ManagerUI::PrintString(std::string& Text, Vector2<float> position)
{
	Vertex a;
	a.color = { 1,1,1,1 };
	a.texId = 13;
	a.position = { position.x,position.y,1.0f };
	for (char digit : Text)
	{
		int x = digit % 16;
		int y = digit / 16;
		y++;
		a.texCords = { x / 16.0f,1 - (y / 16.0f) };
		m_VertexBuffer->Add(a);
		a.position.y += charHeight;
		a.texCords.y += 1 / 16.0f;
		m_VertexBuffer->Add(a);
		a.position.x += charWidth;
		a.texCords.x += 1 / 16.0f;
		m_VertexBuffer->Add(a);
		a.position.y -= charHeight;
		a.texCords.y -= 1 / 16.0f;
		m_VertexBuffer->Add(a);
		m_IndexBuffer->AddRectangle();
		a.position.x -= charWidth - charWidthOffset;
	}
}
void ManagerUI::PrintSquare(Vector2<float> Position, Vector2<float> Size, Vector4<float> Color, float TextureID)
{
	Vertex a;
	a.color = Color;
	a.texId = TextureID;
	a.texCords = { 0,0 };
	a.position = { Position.x,Position.y,1.0f };
	m_VertexBuffer->Add(a);
	a.texCords = { 0,1 };
	a.position.y += Size.y;
	m_VertexBuffer->Add(a);
	a.texCords = { 1,1 };
	a.position.x += Size.x;
	m_VertexBuffer->Add(a);
	a.texCords = { 1,0 };
	a.position.y -= Size.y;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
}
void ManagerUI::UpdateUI()
{
	PrintSquare({ (float)(ScreenWidth - 16) / 2,(float)(ScreenHeight - 16) / 2 }, { 32.0f, 32.0f }, { 1, 1, 1, 1 }, 12);

	if (debugActive)
	{
		Vector2<float> TextPosition = { 0.0f,(float)(ScreenHeight - charHeight - 2) };
		PrintString("FPS:" + ToString(FPS), TextPosition);
		TextPosition.y -= charHeightOffset;
		PrintString("Position:" + ToString((int)GameManager::player->Position.x) + "," + ToString((int)GameManager::player->Position.y) + "," + ToString((int)GameManager::player->Position.z), TextPosition);
		TextPosition.y -= charHeightOffset;
		Block* bl = GameManager::player->GetFacingBlock();
		if (bl != nullptr) {
			TextPosition.y -= charHeightOffset;
			PrintString("Facing block:" + ToString(bl->Transform.x) + "," + ToString(bl->Transform.y) + "," + ToString(bl->Transform.z), TextPosition);
		}
	}
	if (TypingActive)
	{
		Vector2<float> TypingText = { 0.0f,0.0f };
		PrintString(chatbox, TypingText);
		PrintSquare({ 0.0f,0.0f }, { (float)ScreenWidth, charHeight }, { 0.2f, 0.2f, 0.2f, 0.6f }, -1);
	}
	m_VertexBuffer->Bind();
	m_VertexBuffer->Allocate();
	Renderer::DrawGeometry(*m_VertexBuffer, *m_IndexBuffer);
	m_VertexBuffer->Clear();
	m_IndexBuffer->Clear();
}
void ToggleStates(int key, int action)
{
	if (key == GLFW_KEY_F3 && action == GLFW_PRESS && Playing) {
		debugActive = !debugActive;
	}
	if (key == GLFW_KEY_SLASH && action == GLFW_PRESS && Playing) {
		TypingActive = !TypingActive;
		Playing = !Playing;
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS && !Playing)
	{
		TypingActive = !TypingActive;
		Playing = !Playing;
		chatbox = "";
	}
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS && !Playing)
	{
		TypingActive = !TypingActive;
		Playing = !Playing;
		Commands::ExecuteCommand(chatbox);
		chatbox = "";
	}
	if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS && !Playing)
	{
		chatbox.pop_back();
	}
	if (!Playing && action == GLFW_PRESS && key <= 255)
	{
		chatbox += (char)key;
	}
}
void ManagerUI::Init()
{
	m_VertexBuffer = std::make_unique<VertexBuffer>();
	m_IndexBuffer = std::make_unique<IndexBuffer>();
	Input::SetKeyCallback([](GLFWwindow* window, int key, int actioncode, int action, int mods) { ToggleStates(key, action); });
}