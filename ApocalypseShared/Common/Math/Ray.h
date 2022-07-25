#pragma once
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/glm/gtc/type_ptr.hpp"

class Ray {
public:
	Ray(const glm::vec3& position, float pitch, float yaw);

	void step(float scale);

	const glm::vec3& getEnd() const;

	float getLength() const;

	const glm::vec3& getLast() const;

private:
	glm::vec3 m_rayStart;
	glm::vec3 m_rayEnd;
	glm::vec3 m_last;
	float m_pitch;
	float m_yaw;
};
