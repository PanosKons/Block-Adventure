#include "pch.h"
#include "GameScene.h"
#include "Common/World/World.h"
#include "Common/Entities/Player/Player.h"
#include "UI/ManagerUI.h"
#include "GameManager.h"
#include "Renderer.h"
#include "Networking/Networking.h"
#include "Entities/EntityManagerClient.h"
#include "Client.h"
#include "Common/World/WorldManager.h"

void GameScene::Start()
{
	EntityManagerClient::Start();
	WorldManager::BaseWorld = new World();

	Networking::Connect();

	EntityManagerClient::CreateSelf(Networking::Player_id);

	ManagerUI::Init();
}

void GameScene::Update(float deltaTime)
{
	Renderer::SetBackroundColorAndClear({ 0.0f, 0.8f, 1.0f, 1.0f });

	PlayerInput::Update();

	EntityManagerClient::player->Update(deltaTime);
}

void GameScene::Render()
{
	Renderer::SetPlayerView();
	WorldManager::BaseWorld->Render();

	Renderer::SetUIView();
	ManagerUI::UpdateUI();

	Renderer::SetPlayerView();
	EntityManager::Render();

}

void GameScene::End()
{
	ShuttingDown = true;
	EntityManager::ShutDown();
	GameManager::Overworld->Save();
	delete GameManager::player;
	delete GameManager::Overworld;
	Networking::ShutDown();
}
