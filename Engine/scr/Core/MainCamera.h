#pragma once
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/glm/gtc/quaternion.hpp"
#include "../vendor/glm/gtx/quaternion.hpp"
class MainCamera {
public:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	MainCamera();
};