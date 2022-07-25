#pragma once
#include "Common/World/World.h"
#include "Common/Entities/Player/Player.h"
#include "Scenes/Scene.h"
class GameManager
{
public:
	inline static Player* player;
	static void Update(float deltaTime);
	static void Start();
	static void Shutdown();
	static void SetScene(Scene* s);
};
