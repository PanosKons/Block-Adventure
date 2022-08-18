#include "pch.h"
#include "GameManager.h"
#include "Client.h"
#include "Renderer.h"
#include "Scenes/Scene.h"
#include "Scenes/MainMenuScene.h"
#include "Scenes/GameScene.h"

static Scene* scene;

void GameManager::Tick(double TimeStep)
{
	scene->Update(TimeStep);
	scene->Render(TimeStep);
	Renderer::Render();
}
void GameManager::Start()
{
	std::wcin >> Client::ip;
	std::cin >> Client::port;
	Client::ScreenWidth = 1920 / 2;
	Client::ScreenHeight = 1080 / 2;

	Renderer::CreateWindow("GAME");

	scene = (Scene*)new MainMenuScene();
	scene->Start();
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