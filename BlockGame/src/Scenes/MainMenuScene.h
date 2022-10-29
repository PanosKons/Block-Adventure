#pragma once
#include "Scene.h"
#include "Rendering/RenderData.h"

class MainMenuScene : Scene
{
public:
	void Start() override;
	void Update(double TimeStep) override;
	void Render(double TimeStep) override;
	void End() override;
private:
	RenderData m_RenderData;
};

