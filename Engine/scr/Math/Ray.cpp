#include <Engine.h>
#include "Ray.h"

Ray::Ray(const glm::vec3& position, float pitch, float yaw)
	: m_rayStart(position)
	, m_rayEnd(position)
	, m_pitch(pitch)
	, m_yaw(yaw)
{
}

void Ray::step(float scale)
{
	m_last = m_rayEnd;
	glm::vec3 p(glm::cos(glm::radians(m_yaw)), glm::tan(glm::radians(m_pitch)), glm::sin(glm::radians(m_yaw)));
	float magnitude = glm::length(p);
	p /= magnitude;
	p *= scale;
	m_rayEnd += p;
}

const glm::vec3& Ray::getEnd() const
{
	return m_rayEnd;
}
const glm::vec3& Ray::getLast() const
{
	return m_last;
}

float Ray::getLength() const
{
	return glm::distance(m_rayStart, m_rayEnd);
}