#pragma once
#include "Common/World/World.h"
#include "Common/Entities/Player/Player.h"
#include "Scenes/Scene.h"
class GameManager
{
public:
	static void Tick(double StepTime);
	static void Start();
	static void Shutdown();
	static void SetScene(Scene* s);
};
