#pragma once
#include "Scene.h"
class MainMenuScene : Scene
{
public:
	void Start() override;
	void Update(float deltaTime) override;
	void Render() override;
	void End() override;
};

