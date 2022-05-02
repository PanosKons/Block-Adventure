#include <Engine.h>
#include "Renderer.h"
#include "Engine.h"
#include "GameManager.h"
#include "GlobalVariables.h"
#include "SavingData.h"
int main()
{
	SavingData::ActivateLoading(false);
	ScreenWidth = 1920;
	ScreenHeight = 1080;
	Renderer::CreateWindow("GAME");
	Started = true;
	GameManager::Start();
	Renderer::Run();
	return 0;
}
#include <windows.h>
#undef CreateWindow
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PWSTR pCmdLine, int nCmdShow)
{
	SavingData::ActivateLoading(true);
	ScreenWidth = 1920;
	ScreenHeight = 1080;
	Renderer::CreateWindow("GAME");
	Started = true;
	GameManager::Start();
	Renderer::Run();
	return 0;
}