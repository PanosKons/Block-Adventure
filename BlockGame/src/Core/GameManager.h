#pragma once
#include "World.h"
#include "Player.h"
#include "Scene.h"
class GameManager
{
public:
	inline static World* Overworld;
	inline static Player* player;
	static void Update(float deltaTime);
	static void Start();
	static void Shutdown();
	static void SetScene(Scene* s);
};
