#include "ServerManager.h"
#include "pch.h"
#include "Networking.h"
#include "Common/World/World.h"
#include "Server/Server.h"
#include "Common/World/WorldManager.h"
#include "EntityManagerServer.h"
#include "Common/Entities/Player/Player.h"
#include "Logger.h"

static std::thread* ListeningThread;

void ServerManager::Start()
{
	INFO("Starting server...");
	EntityManagerServer::Start();
	WorldManager::BaseWorld = new World();
	ListeningThread = new std::thread(Networking::ListenForClients);
}

void ServerManager::Tick()
{

}

void ServerManager::Shutdown()
{
	INFO("Stopping server...");
	EntityManagerServer::ShutDown();
	ListeningThread->join();
	delete ListeningThread;
	delete WorldManager::BaseWorld;
}
