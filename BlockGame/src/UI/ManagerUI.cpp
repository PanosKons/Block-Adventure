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
	//if (TypingActive)
	//{
	//	Vector3<float> TypingText = { 0.0f,0.0f,BaseLayer };
	//	PrintString(chatbox, TypingText);
	//	PrintSquare({ 0.0f,0.0f,BaseLayer + 0.1f }, { (float)Client::ScreenWidth, charHeight }, { 0.2f, 0.2f, 0.2f, 0.6f }, -1);
	//}
}
void ToggleStates(int key, int action)
{
	/*
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