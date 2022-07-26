#include "ServerManager.h"
#include <pch.h>
#include "Networking.h"
#include "Common/World/World.h"
#include "Server/Server.h"
#include "Common/World/WorldManager.h"
static std::thread* ListeningThread;
void ServerManager::Start()
{
	WorldManager::BaseWorld = new World();
	ListeningThread = new std::thread(Networking::ListenForClients);
}

void ServerManager::Tick()
{
}

void ServerManager::Shutdown()
{
	delete ListeningThread;
	delete WorldManager::BaseWorld;
}
