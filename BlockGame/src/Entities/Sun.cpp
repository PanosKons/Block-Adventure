#include "Sun.h"
#include "EntityManagerClient.h"

void Sun::Update()
{
	angle += 0.001f;
}

Vector3<double> Sun::GetPosition()
{
	//Vector3<double>& position = EntityManagerClient::GetPlayer().Position;
	Vector3<double> position = { 2600,70,2600 };
	return { 20 * cos(angle) + (float)position.x, 20 * sin(angle) + (float)position.y, (float)position.z };
}

Vector3<float> Sun::GetDirection()
{
	return { cos(angle), sin(angle), 0};
}
