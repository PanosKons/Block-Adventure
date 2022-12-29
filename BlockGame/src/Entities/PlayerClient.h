#pragma once
#include "Common\Entities\Player\Player.h"
#include "vendor/glm/gtc/quaternion.hpp"
#include "vendor/glm/gtx/quaternion.hpp"
class PlayerClient : public Player
{
public:
	PlayerClient(Credentials& credentials);
	void CursorMoved(double xpos, double ypos);
	glm::vec3 GetCameraFront();
	glm::vec3 GetCameraPosition();
	void InputTick(double TimeStep);
	float lastX = 0.0f;
	float lastY = 0.0f;
	float Fov = 70.0f;
	bool firstMouse = true;
	bool IsGUIOpen = false;
};

