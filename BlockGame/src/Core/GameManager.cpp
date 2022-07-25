#include "pch.h"
#include "GameManager.h"
#include "Client.h"
#include "UI/ManagerUI.h"
#include "Timer.h"
#include "SavingData.h"
#include "Renderer.h"
#include "Scenes/Scene.h"
#include "Scenes/MainMenuScene.h"
#include "Scenes/GameScene.h"
static float second;
static int FrameCount = 0;
static Scene* scene;
void GameManager::Update(float deltaTime)
{
	//Calculate FPS
	if (second > 1)
	{
		Client::FPS = FrameCount;
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
	std::wcin >> Client::ip;
	std::cin >> Client::port;
	Client::ScreenWidth = 1920;
	Client::ScreenHeight = 1080;
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