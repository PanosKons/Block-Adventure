#include "GameManager.h"
#include "Client.h"
#include "Logger.h"

static auto Now = std::chrono::system_clock::now();
static auto LastStep = std::chrono::system_clock::now();

int Main()
{
	INFO("Starting client...");
	GameManager::Start();
	while (!Client::ShouldStop)
	{
		Now = std::chrono::system_clock::now();
		std::chrono::duration<double> StepDelta = Now - LastStep;
		LastStep = Now;
		if (IdealTimeStep > StepDelta.count())
		{
			double WaitTime = (IdealTimeStep - StepDelta.count()) * 1000.0 * 1000.0;
			std::this_thread::sleep_for(std::chrono::microseconds((int)WaitTime));
			GameManager::Tick(IdealTimeStep);
		}
		else
		{
			GameManager::Tick(StepDelta.count());
		}
	}
	GameManager::Shutdown();
	INFO("Shutting client...");
	return 0;
}




int main()
{
	return Main();
}

#define NOMINMAX
#include <windows.h>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PWSTR pCmdLine, int nCmdShow)
{
	return Main();
}