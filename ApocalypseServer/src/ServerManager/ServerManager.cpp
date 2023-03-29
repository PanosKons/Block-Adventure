#include "ServerManager.h"
#include "pch.h"
#include "NetworkingServer.h"
#include "Common/World/World.h"
#include "Server/Server.h"
#include "World/WorldManagerServer.h"
#include "EntityManagerServer.h"
#include "Common/Entities/Player/Player.h"
#include "Logger.h"
#include "ScriptingManager.h"

static std::thread* ListeningThread;

void ServerManager::Start()
{
	INFO("Starting server...");
	ScriptingManager::Start();
	EntityManagerServer::Start();
	WorldManager::BaseWorld = new World();
	ListeningThread = new std::thread(NetworkingServer::ListenForClients);
}

void ServerManager::Tick()
{
	WorldManagerServer::SendAppropriateChunks();
	WorldManagerServer::RemoveUnusedChunks();
	ScriptingManager::UpdateEvent();
}

void ServerManager::Shutdown()
{
	INFO("Stopping server...");
	EntityManagerServer::ShutDown();
	ListeningThread->join();
	delete ListeningThread;
	delete WorldManager::BaseWorld;
}
