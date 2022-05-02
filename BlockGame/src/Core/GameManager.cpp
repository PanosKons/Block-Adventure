#include <Engine.h>
#include "GameManager.h"
#include "GlobalVariables.h"
#include <iostream>
#include "ManagerUI.h"
#include "Timer.h"
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
	Overworld->Render();
	player->Update(deltaTime);
}
void GameManager::Start()
{
	new World(0);
	player = new Player();
	ManagerUI::Init();
}
void GameManager::Shutdown()
{
	Overworld->Save();
	delete player;
	delete Overworld;
}