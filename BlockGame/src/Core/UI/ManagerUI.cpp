#include <Engine.h>
#include "ManagerUI.h"
#include "Renderer.h"
#include "GameManager.h"
#include "World.h"
#include <sstream>
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "GlobalVariables.h"
#include "Input.h"
#include <iostream>
#include "Commands.h"
#include "util/StringConvertions.h"
static std::unique_ptr<IndexBuffer> m_IndexBuffer;
static std::unique_ptr<VertexBuffer> m_VertexBuffer;
static bool debugActive = false;
static bool TypingActive = false;
static std::string chatbox;
static std::string lastCommand;
constexpr float SlotWidth = 64.0f;
constexpr float SlotHeight = 64.0f;
constexpr float BaseLayer = 10.0f;
std::string ManagerUI::ToString(bool value)
{
	if (value)
		return "true";
	else return "false";
}
void ManagerUI::PrintString(std::string Text, Vector3<float> position)
{
	Renderer::DrawText(*m_VertexBuffer, *m_IndexBuffer, Text, position);
}
void ManagerUI::PrintSquare(Vector3<float> Position, Vector2<float> Size, Vector4<float> Color, float TextureID)
{
	Renderer::DrawSquare(*m_VertexBuffer, *m_IndexBuffer, Position, Size, Color, { 0,0 }, {1,1}, TextureID);
}
void ManagerUI::PrintSquare(Vector3<float> Position, Vector2<float> Size, Vector4<float> Color, Vector2<float> TexCords, Vector2<float> TexSize, float TextureID)
{
	Renderer::DrawSquare(*m_VertexBuffer,*m_IndexBuffer,Position, Size, Color, TexCords, TexSize, TextureID);
}
void ManagerUI::UpdateUI()
{
	Vector3<float> SlotPosition = { (ScreenWidth / 2) - (4.5f * SlotWidth),0.0f,BaseLayer - 0.3f };
	for (int i = 0; i < 9; i++)
	{
		PrintSquare(SlotPosition, { SlotWidth,SlotHeight }, { 1,1,1,1 }, 14);
		SlotPosition.x += SlotWidth;
	}
	SlotPosition = { (ScreenWidth / 2) - (4.5f * SlotWidth),0.0f,BaseLayer - 0.2f };
	for (int i = 0; i < 9; i++)
	{
		if (GameManager::player->Inventory[i].count != 0)
		{
			unsigned char a = GetTexture(GameManager::player->Inventory[i].id, GameManager::player->Inventory[i].type)[0];
			PrintSquare({ SlotPosition.x + 8, SlotPosition.y + 8,SlotPosition.z }, { SlotWidth - 16,SlotHeight - 16 }, { 1,1,1,1 }, { (a % 16) / 16.0f, (a / 16) / 16.0f }, { 1 / 16.0f, 1 / 16.0f }, 0.0f);
		}
		SlotPosition.x += SlotWidth;
	}
	SlotPosition = { (ScreenWidth / 2) - (4.5f * SlotWidth),0.0f,BaseLayer - 0.1f };
	for (int i = 0; i < 9; i++)
	{
		if (GameManager::player->Inventory[i].count > 1)
			PrintString(StringConvertions::ToString(GameManager::player->Inventory[i].count), SlotPosition);
		SlotPosition.x += SlotWidth;
	}
	SlotPosition = { (ScreenWidth / 2) - (4.5f * SlotWidth),0.0f,BaseLayer - 0.05f };
	PrintSquare({ SlotPosition.x + SlotWidth * GameManager::player->ActiveSlot,SlotPosition.y,SlotPosition.z }, { SlotWidth,SlotHeight }, { 1,1,1,1 }, 15);
	PrintSquare({ (float)(ScreenWidth - 16) / 2,(float)(ScreenHeight - 16) / 2,BaseLayer }, { 32.0f, 32.0f }, { 1, 1, 1, 1 }, 12);
	if (debugActive)
	{
		Vector3<float> TextPosition = { 0.0f,(float)(ScreenHeight - charHeight - 2),BaseLayer };
		PrintString("FPS:" + StringConvertions::ToString(FPS), TextPosition);
		TextPosition.y -= charHeightOffset;
		PrintString("Position:" + StringConvertions::ToString((int)GameManager::player->Position.x) + "," + StringConvertions::ToString((int)GameManager::player->Position.y) + "," + StringConvertions::ToString((int)GameManager::player->Position.z), TextPosition);
		TextPosition.y -= charHeightOffset;
		PrintString("ChunkPosition:" + StringConvertions::ToString((int)GameManager::player->ChunkPosition.x) + "," + StringConvertions::ToString((int)GameManager::player->ChunkPosition.y), TextPosition);
		Block* bl = GameManager::player->GetFacingBlock();
		if (bl != nullptr) {
			TextPosition.y -= charHeightOffset;
			PrintString("Facing block:" + StringConvertions::ToString(bl->Position.x) + "," + StringConvertions::ToString(bl->Position.y) + "," + StringConvertions::ToString(bl->Position.z), TextPosition);
		}
	}
	if (TypingActive)
	{
		Vector3<float> TypingText = { 0.0f,0.0f,BaseLayer };
		PrintString(chatbox, TypingText);
		PrintSquare({ 0.0f,0.0f,BaseLayer + 0.1f }, { (float)ScreenWidth, charHeight }, { 0.2f, 0.2f, 0.2f, 0.6f }, -1);
	}
	m_VertexBuffer->Bind();
	m_VertexBuffer->Allocate();
	Renderer::DrawGeometry(*m_VertexBuffer, *m_IndexBuffer);
	m_VertexBuffer->Clear();
	m_IndexBuffer->Clear();
}
void ToggleStates(int key, int action)
{
	if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9 && action == GLFW_PRESS && Playing)
	{
		GameManager::player->ActiveSlot = key - 49;
	}
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
	if (key == GLFW_KEY_UP && action == GLFW_PRESS && !Playing)
	{
		chatbox = lastCommand;
	}
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS && !Playing)
	{
		TypingActive = !TypingActive;
		Playing = !Playing;
		Commands::ExecuteCommand(chatbox);
		lastCommand = chatbox;
		chatbox = "";
	}
	if (key == GLFW_KEY_BACKSPACE && !Playing)
	{
		if (!chatbox.empty())
			chatbox.pop_back();
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS && Playing) {
		TypingActive = !TypingActive;
		Playing = !Playing;
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS && Playing) {
		GameManager::player->fov = 30.0f;
	}
	else if (key == GLFW_KEY_C && action == GLFW_RELEASE && Playing)
	{
		GameManager::player->fov = 70.0f;
	}
	
}
void TextInput(unsigned int codepoint)
{
	if (!Playing && codepoint <= 255)
	{
		chatbox += (char)codepoint;
	}
}
void ManagerUI::Init()
{
	m_VertexBuffer = std::make_unique<VertexBuffer>();
	m_IndexBuffer = std::make_unique<IndexBuffer>();
	Input::SetKeyCallback([](GLFWwindow* window, int key, int actioncode, int action, int mods) { ToggleStates(key, action); });
	Input::SetCharCallback([](GLFWwindow* window, unsigned int key) {TextInput(key); });
}