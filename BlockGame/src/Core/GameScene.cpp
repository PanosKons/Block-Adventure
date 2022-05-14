#include "GameScene.h"
#include "World.h"
#include "Player.h"
#include "ManagerUI.h"
#include "GameManager.h"
#include "Renderer.h"
#include "Networking.h"
void GameScene::Start()
{
	Networking::Connect();

	new World(0);
	GameManager::player = new Player();
	ManagerUI::Init();
}

void GameScene::Update(float deltaTime)
{
	Renderer::SetBackroundColorAndClear({ 0.0f, 0.8f, 1.0f, 1.0f });

	Renderer::SetPlayerView();
	GameManager::Overworld->Render();
	GameManager::player->Update(deltaTime);

	Renderer::SetUIView();
	ManagerUI::UpdateUI();
}

void GameScene::Render()
{
}

void GameScene::End()
{
	GameManager::Overworld->Save();
	delete GameManager::player;
	delete GameManager::Overworld;
	Networking::ShutDown();
}
