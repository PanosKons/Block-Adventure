#include <Engine.h>
#include "Renderer.h"
#include "Engine.h"
#include "GameManager.h"
#include "GlobalVariables.h"
#include "SavingData.h"
int main()
{
	SavingData::ActivateLoading(false);
	ScreenWidth = 1440;
	ScreenHeight = 960;
	Renderer::CreateWindow("GAME");
	GameManager::Start();
	Renderer::Run();
	return 0;
}