#pragma once
#include "Scene.h"
class GameScene : Scene
{
public:
	GameScene() {};
	void Start() override;
	void Update(double TimeStep) override;
	void Render(double TimeStep) override;
	void End() override;
};

