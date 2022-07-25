#include "pch.h"
#include "ServerManager/ServerManager.h"
#include "Server/Server.h"
int main()
{
	ServerManager::Start();
	while (!Server::ShouldStop)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(20)); // 50 ticks per second
		ServerManager::Tick();
	}
	ServerManager::Shutdown();
	return 0;
}