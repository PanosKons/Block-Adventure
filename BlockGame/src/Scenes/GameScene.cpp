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
#include "RendererClient.h"

void GameScene::Start()
{
	EntityManagerClient::Start();
	WorldManager::BaseWorld = new World();

	Networking::Connect();

	ManagerUI::Init();
}

void GameScene::Update(float deltaTime)
{
	EntityManagerClient::GetPlayer().Update();
}

void GameScene::Render()
{
	Renderer::SetBackroundColorAndClear({ 0.0f, 0.8f, 1.0f, 1.0f });

	RendererClient::RenderWorld(WorldManager::BaseWorld);
	RendererClient::RenderEntities();
	RendererClient::RenderUI();

	Renderer::SetPlayerView();
	WorldManager::BaseWorld->Render();

	Renderer::SetUIView();
	ManagerUI::UpdateUI();

	Renderer::SetPlayerView();
	EntityManager::Render();
}

void GameScene::End()
{
	EntityManager::ShutDown();
	Networking::ShutDown();
	delete WorldManager::BaseWorld;
}
