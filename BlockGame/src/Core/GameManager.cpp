#include "pch.h"
#include "GameManager.h"
#include "Client.h"
#include "Renderer.h"
#include "Scenes/Scene.h"
#include "Scenes/MainMenuScene.h"
#include "Scenes/GameScene.h"

static Scene* scene;

void GameManager::Tick(double StepTime)
{
	scene->Update((float)StepTime);
	scene->Render();
}
void GameManager::Start()
{
	std::wcin >> Client::ip;
	std::cin >> Client::port;
	Client::ScreenWidth = 1920;
	Client::ScreenHeight = 1080;

	scene = (Scene*)new MainMenuScene();
	scene->Start();

	Renderer::CreateWindow("GAME");
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