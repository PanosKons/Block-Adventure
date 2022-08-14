#pragma once
#include "Scene.h"
#include "Rendering/RenderData.h"

class MainMenuScene : Scene
{
public:
	void Start() override;
	void Update(float deltaTime) override;
	void Render() override;
	void End() override;
private:
	RenderData renderData;
};

