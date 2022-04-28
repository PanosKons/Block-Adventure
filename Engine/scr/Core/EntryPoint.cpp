#include "../Engine.h"
#include "GameManager.h"
#include "ManagerUI.h"
#include "GlobalVariables.h"
int WinMain()
{
	ScreenWidth = 1440;
	ScreenHeight = 960;
	Renderer::CreateWindow("GAME");
	GameManager::Start();
	ManagerUI::Init();
	Renderer::Run();
	return 0;
}