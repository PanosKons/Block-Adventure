#pragma once
#include "Common\Entities\Player\Player.h"

enum class Screen
{
	Game, ChatBox, GUI
};

class PlayerClient : public Player
{
public:
	PlayerClient(Credentials& credentials);

	void CursorMoved(double xpos, double ypos);
	void KeyPressed(int key, int action);
	void TextInput(int codepoint);
	void MousePressed(int mouse, int action);

	void InputTick(double TimeStep);

	float Fov = 70.0f;

	float lastX = 0.0f;
	float lastY = 0.0f;
	bool firstMouse = true;

	Screen currentScreen = Screen::Game;

	Gui activeGui;
	Inventory activeInventory;
	int selectedSlot = -1;

	std::string chatbox;
	std::string lastCommand;
};

