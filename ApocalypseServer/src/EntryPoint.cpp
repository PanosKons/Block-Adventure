#include "pch.h"
#include "ServerManager/ServerManager.h"
#include "Server/Server.h"
#include "Common/World/WorldConstants.h"

int main()
{
	ServerManager::Start();
	while (!Server::ShouldStop)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds((int)(TimeStep * 1000)));
		ServerManager::Tick();
	}
	ServerManager::Shutdown();
	return 0;
}