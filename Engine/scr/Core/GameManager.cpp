#include "GameManager.h"
#include "GlobalVariables.h"
#include <iostream>
static float second;
static int FrameCount = 0;
void GameManager::Update(float deltaTime)
{
	if (second > 1)
	{
		FPS = FrameCount;
		second = 0;
		FrameCount = 0;
	}
	FrameCount++;
	second += deltaTime;
	player->Update(deltaTime);
	Overworld->Render();
}
void GameManager::Start()
{
	player = new Player();
	new World(0);
}
void GameManager::Shutdown()
{
	delete player;
	delete Overworld;
}