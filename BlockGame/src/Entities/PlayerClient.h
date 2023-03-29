#pragma once
#include "Common\Entities\Player\Player.h"

class PlayerClient : public Player
{
public:
	PlayerClient(Credentials& credentials);

	void CursorMoved(double xpos, double ypos);
	void KeyPressed(int key, int action);
	void TextInput(int codepoint);

	void InputTick(double TimeStep);

	float Fov = 70.0f;

	float lastX = 0.0f;
	float lastY = 0.0f;
	bool firstMouse = true;
	bool IsGUIOpen = false;

	std::string chatbox;
	std::string lastCommand;
};

