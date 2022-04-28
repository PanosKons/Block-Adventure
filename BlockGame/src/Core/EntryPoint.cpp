#include <Engine.h>
#include "Renderer.h"
#include "Engine.h"
#include "GameManager.h"
#include "GlobalVariables.h"
int main()
{
	ScreenWidth = 1440;
	ScreenHeight = 960;
	Renderer::CreateWindow("GAME");
	GameManager::Start();
	Renderer::Run();
	return 0;
}