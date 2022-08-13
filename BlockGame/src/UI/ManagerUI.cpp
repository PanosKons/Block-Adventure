#include "pch.h"
#include "ManagerUI.h"
#include "Renderer.h"
#include "GameManager.h"
#include "Common/World/World.h"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Input.h"
#include "Commands.h"
#include "Common/Math/StringConvertions.h"
#include "Entities/EntityManagerClient.h"
#include "Client.h"

static std::unique_ptr<IndexBuffer> m_IndexBuffer;
static std::unique_ptr<VertexBuffer> m_VertexBuffer;
static bool debugActive = false;
static bool TypingActive = false;
static std::string chatbox;
static std::string lastCommand;
constexpr float SlotWidth = 64.0f;
constexpr float SlotHeight = 64.0f;
constexpr float BaseLayer = 10.0f;
constexpr float SlotsX = 64.0f;
std::string ManagerUI::ToString(bool value)
{
	if (value)
		return "true";
	else return "false";
}
void ManagerUI::PrintString(std::string Text, Vector3<float> position)
{
	//Renderer::DrawText(*m_VertexBuffer, *m_IndexBuffer, Text, position);
}
void ManagerUI::PrintSquare(Vector3<float> Position, Vector2<float> Size, Vector4<float> Color, float TextureID)
{
	//Renderer::DrawSquare(*m_VertexBuffer, *m_IndexBuffer, Position, Size, Color, { 0,0 }, {1,1}, TextureID);
}
void ManagerUI::PrintSquare(Vector3<float> Position, Vector2<float> Size, Vector4<float> Color, Vector2<float> TexCords, Vector2<float> TexSize, float TextureID)
{
	//Renderer::DrawSquare(*m_VertexBuffer,*m_IndexBuffer,Position, Size, Color, TexCords, TexSize, TextureID);
}
void ManagerUI::UpdateUI()
{
	/*
	Vector3<float> SlotPosition = { SlotsX,0.0f,BaseLayer - 0.3f };

	Vector3<float> HealthBarPosition = { SlotsX,10.0f + SlotHeight, BaseLayer };
	PrintSquare(HealthBarPosition, { 320.0f,40.0f }, { 1.0f,0.0f,0.0f,1.0f }, -1);
	PrintSquare(HealthBarPosition, { 320.0f * EntityManagerClient::GetPlayer().Health / EntityManagerClient::GetPlayer().MaxHealth,40.0f }, { 0.0f,1.0f,0.0f,1.0f }, -1);
	PrintString(StringConvertions::ToString((int)EntityManagerClient::GetPlayer().Health) + "/" + StringConvertions::ToString((int)EntityManagerClient::GetPlayer().MaxHealth), HealthBarPosition);
	for (int i = 0; i < InventorySize; i++)
	{
		PrintSquare(SlotPosition, { SlotWidth,SlotHeight }, { 1,1,1,1 }, 14);
		SlotPosition.x += SlotWidth;
	}
	SlotPosition = { SlotsX,0.0f,BaseLayer - 0.2f };
	for (int i = 0; i < InventorySize; i++)
	{
		if (EntityManagerClient::GetPlayer().Inventory[i].count != 0)
		{
			unsigned char a = GetTexture(EntityManagerClient::GetPlayer().Inventory[i].id, EntityManagerClient::GetPlayer().Inventory[i].type)[0];
			PrintSquare({ SlotPosition.x + 8, SlotPosition.y + 8,SlotPosition.z }, { SlotWidth - 16,SlotHeight - 16 }, { 1,1,1,1 }, { (a % 16) / 16.0f, (a / 16) / 16.0f }, { 1 / 16.0f, 1 / 16.0f }, 0.0f);
		}
		SlotPosition.x += SlotWidth;
	}
	SlotPosition = { SlotsX,0.0f,BaseLayer - 0.1f };
	for (int i = 0; i < InventorySize; i++)
	{
		if (EntityManagerClient::GetPlayer().Inventory[i].count > 1)
			PrintString(StringConvertions::ToString(EntityManagerClient::GetPlayer().Inventory[i].count), SlotPosition);
		SlotPosition.x += SlotWidth;
	}
	SlotPosition = { SlotsX,0.0f,BaseLayer - 0.05f };
	PrintSquare({ SlotPosition.x + SlotWidth * EntityManagerClient::GetPlayer().ActiveSlot,SlotPosition.y,SlotPosition.z }, { SlotWidth,SlotHeight }, { 1,1,1,1 }, 15);
	PrintSquare({ (float)(Client::ScreenWidth - 16) / 2,(float)(Client::ScreenHeight - 16) / 2,BaseLayer }, { 32.0f, 32.0f }, { 1, 1, 1, 1 }, 12);
	if (debugActive)
	{
		Vector3<float> TextPosition = { 0.0f,(float)(ScreenHeight - charHeight - 2),BaseLayer };
		PrintString("FPS:" + StringConvertions::ToString(FPS), TextPosition);
		TextPosition.y -= charHeightOffset;
		PrintString("Position:" + StringConvertions::ToString((int)EntityManagerClient::GetPlayer().Position.x) + "," + StringConvertions::ToString((int)GameManager::player->Position.y) + "," + StringConvertions::ToString((int)GameManager::player->Position.z), TextPosition);
		Block bl = GameManager::player->GetFacingBlock();
		if (bl.data != nullptr) {
			TextPosition.y -= charHeightOffset;
			PrintString("Facing block:" + StringConvertions::ToString(bl.Position.x) + "," + StringConvertions::ToString(bl.Position.y) + "," + StringConvertions::ToString(bl.Position.z), TextPosition);
		}
	}
	if (TypingActive)
	{
		Vector3<float> TypingText = { 0.0f,0.0f,BaseLayer };
		PrintString(chatbox, TypingText);
		PrintSquare({ 0.0f,0.0f,BaseLayer + 0.1f }, { (float)Client::ScreenWidth, charHeight }, { 0.2f, 0.2f, 0.2f, 0.6f }, -1);
	}
	m_VertexBuffer->Bind();
	m_VertexBuffer->Allocate();
	Renderer::DrawGeometry(*m_VertexBuffer, *m_IndexBuffer);
	m_VertexBuffer->Clear();
	m_IndexBuffer->Clear();
	*/
}
void ToggleStates(int key, int action)
{
	/*
	if (key >= Key::n1 && key <= Key::n9 && action == Action::Press && Playing)
	{
		EntityManagerClient::GetPlayer().ActiveSlot = key - 49;
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
		EntityManagerClient::GetPlayer().Fov = 30.0f;
	}
	else if (key == GLFW_KEY_C && action == GLFW_RELEASE && Playing)
	{
		EntityManagerClient::GetPlayer().Fov = 70.0f;
	}
	*/
}
void TextInput(unsigned int codepoint)
{
	//if (!Playing && codepoint <= 255)
	//{
	//	chatbox += (char)codepoint;
	//}
}
void ManagerUI::Init()
{
	//m_VertexBuffer = std::make_unique<VertexBuffer>();
	//m_IndexBuffer = std::make_unique<IndexBuffer>();
	//Input::SetKeyCallback([](GLFWwindow* window, int key, int actioncode, int action, int mods) { ToggleStates(key, action); });
	//Input::SetCharCallback([](GLFWwindow* window, unsigned int key) {TextInput(key); });
}