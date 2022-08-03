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
	RendererClient::RenderWorld(WorldManager::BaseWorld);
	RendererClient::RenderEntities();
	RendererClient::RenderUI();
	Renderer::Render();
}

void GameScene::End()
{
	EntityManager::ShutDown();
	Networking::ShutDown();
	delete WorldManager::BaseWorld;
}
