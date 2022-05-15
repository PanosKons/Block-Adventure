#include <Engine.h>
#include "GameManager.h"
#include "GlobalVariables.h"
#include "ManagerUI.h"
#include "Timer.h"
#include "SavingData.h"
#include "Renderer.h"
#include "Scene.h"
#include "MainMenuScene.h"
#include "GameScene.h"
static float second;
static int FrameCount = 0;
static Scene* scene;
void GameManager::Update(float deltaTime)
{
	//Calculate FPS
	if (second > 1)
	{
		FPS = FrameCount;
		second = 0;
		FrameCount = 0;
	}
	FrameCount++;
	second += deltaTime;

	scene->Update(deltaTime);
	scene->Render();
}
void GameManager::Start()
{
	std::wcin >> ip;
	std::cin >> port;
	SavingData::ActivateLoading(true); //Debug function
	ScreenWidth = 1920;
	ScreenHeight = 1080;
	Renderer::CreateWindow("GAME");

	scene = (Scene*)new MainMenuScene();
	scene->Start();

	Renderer::Run();
	GameManager::Shutdown();
}
void GameManager::SetScene(Scene* s)
{
	if (scene != s)
	{
		scene->End();
		delete scene;
		scene = s;
		scene->Start();
	}
}
void GameManager::Shutdown()
{
	scene->End();
	delete scene;
}