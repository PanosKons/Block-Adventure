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
	Networking::Connect();
	EntityManagerClient::Start();

	WorldManager::BaseWorld = new World();
	GameManager::player = new Player();
	ManagerUI::Init();
}

void GameScene::Update(float deltaTime)
{

	Renderer::SetBackroundColorAndClear({ 0.0f, 0.8f, 1.0f, 1.0f });
	Networking::SendData(PACKET_ID::PlayerPosition,(char*)&GameManager::player->Position,sizeof(GameManager::player->Position));

	Renderer::SetPlayerView();
	WorldManager::BaseWorld->Render();
	GameManager::player->Update(deltaTime);

	Renderer::SetUIView();
	ManagerUI::UpdateUI();
}

void GameScene::Render()
{
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
