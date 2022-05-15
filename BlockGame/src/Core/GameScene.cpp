#include "GameScene.h"
#include "World.h"
#include "Player.h"
#include "ManagerUI.h"
#include "GameManager.h"
#include "Renderer.h"
#include "Networking.h"
#include "EntityManager.h"
void GameScene::Start()
{
	Networking::Connect();
	EntityManager::Start();
	new World(0);
	GameManager::player = new Player();
	ManagerUI::Init();
}

void GameScene::Update(float deltaTime)
{

	Renderer::SetBackroundColorAndClear({ 0.0f, 0.8f, 1.0f, 1.0f });
	Networking::SendData(PACKET_ID::PlayerPosition,(char*)&GameManager::player->Position,sizeof(GameManager::player->Position));


	Renderer::SetPlayerView();
	GameManager::Overworld->Render();
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
	EntityManager::ShutDown();
	GameManager::Overworld->Save();
	delete GameManager::player;
	delete GameManager::Overworld;
	Networking::ShutDown();
}
