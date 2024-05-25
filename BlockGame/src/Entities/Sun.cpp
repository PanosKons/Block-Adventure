#include "Sun.h"
#include "EntityManagerClient.h"

void Sun::Update()
{
	//angle += 0.001f;
}

Vector3<double> Sun::GetPosition()
{
	Vector3<double>& position = EntityManagerClient::GetPlayer().Position;
	return { 200 * cos(angle) + (float)floor(position.x), 200 * sin(angle) + (float)floor(position.y), (float)floor(position.z) };
}

Vector3<float> Sun::GetDirection()
{
	return { cos(angle), sin(angle), 0};
}
