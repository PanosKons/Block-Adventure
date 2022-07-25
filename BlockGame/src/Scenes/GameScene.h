#pragma once
#include "Scene.h"
class GameScene : Scene
{
public:
	GameScene() {};
	void Start() override;
	void Update(float deltaTime) override;
	void Render() override;
	void End() override;
};

