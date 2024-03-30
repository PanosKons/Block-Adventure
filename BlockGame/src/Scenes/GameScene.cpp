#include "pch.h"
#include "GameScene.h"
#include "Common/World/World.h"
#include "Common/Entities/Player/Player.h"
#include "GameManager.h"
#include "Renderer.h"
#include "Networking/NetworkingClient.h"
#include "Entities/EntityManagerClient.h"
#include "Client.h"
#include "World/WorldManagerClient.h"
#include "RendererClient.h"
#include "Logger.h"
#include "Entities/Sun.h"

void GameScene::Start()
{
	INFO("GameScene Started");
	EntityManagerClient::Start();
	WorldManager::BaseWorld = new World();
	NetworkingClient::Connect();
	Renderer::HideCursor(true);
}

void GameScene::Update(double TimeStep)
{
	EntityManagerClient::Tick();
	EntityManagerClient::GetPlayer().InputTick(TimeStep);
	Sun::Update();
}

void GameScene::Render(double TimeStep)
{
	RendererClient::RenderWorld(WorldManager::BaseWorld);
	RendererClient::RenderEntities();
	RendererClient::RenderUI(TimeStep);
}

void GameScene::End()
{
	EntityManager::ShutDown();
	NetworkingClient::ShutDown();
	delete WorldManager::BaseWorld;
}
