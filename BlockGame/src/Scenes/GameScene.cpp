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
#include "World/WorldManagerClient.h"
#include "RendererClient.h"
#include "Logger.h"

void GameScene::Start()
{
	INFO("GameScene Started");
	EntityManagerClient::Start();
	WorldManager::BaseWorld = new World();
	Networking::Connect();
	Renderer::HideCursor(true);
}

void GameScene::Update(float deltaTime)
{
	WorldManagerClient::RequestNewChunks();
	//EntityManagerClient::GetPlayer().InputTick(deltaTime);
}

void GameScene::Render()
{
	RendererClient::RenderWorld(WorldManager::BaseWorld);
	RendererClient::RenderEntities();
	RendererClient::RenderUI();
}

void GameScene::End()
{
	EntityManager::ShutDown();
	Networking::ShutDown();
	delete WorldManager::BaseWorld;
}
