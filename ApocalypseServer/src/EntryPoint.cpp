#include "pch.h"
#include "ServerManager/ServerManager.h"
#include "Server/Server.h"
#include "Common/World/WorldConstants.h"

static auto Now = std::chrono::system_clock::now();
static auto LastStep = std::chrono::system_clock::now();

int main()
{
	ServerManager::Start();
	while (!Server::ShouldStop)
	{
		Now = std::chrono::system_clock::now();
		std::chrono::duration<double> StepDelta = Now - LastStep;
		LastStep = Now;
		if (IdealTimeStep > StepDelta.count() * 1000.0)
		{
			double WaitTime = (IdealTimeStep - StepDelta.count() * 1000.0) * 1000.0;
			std::this_thread::sleep_for(std::chrono::nanoseconds((int)WaitTime));
		}
		ServerManager::Tick();
	}
	ServerManager::Shutdown();
	return 0;
}