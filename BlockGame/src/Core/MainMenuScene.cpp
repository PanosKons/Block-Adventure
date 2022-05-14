#include "MainMenuScene.h"
#include "Renderer.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "GlobalVariables.h"
#include "Input.h"
#include "GameManager.h"
#include "GameScene.h"
void MainMenuScene::Start()
{
}

void MainMenuScene::Update(float deltaTime)
{
	if (Input::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		Scene* scene = (Scene*)new GameScene();
		GameManager::SetScene(scene);
	}
}

void MainMenuScene::Render()
{
	Renderer::SetBackroundColorAndClear({ 0.2f, 0.2f, 0.2f, 1.0f });
	Renderer::SetUIView();
	static VertexBuffer vb;
	static IndexBuffer ib;
	vb.Clear();
	ib.Clear();
	Renderer::DrawSquare(vb, ib, { (float)ScreenWidth / 2,(float)ScreenHeight / 2 }, { 100.0f,100.0f }, { 1.0f,1.0f,1.0f,1.0f }, {0,0},{1,1}, -1);
	vb.Bind();
	vb.Allocate();
	Renderer::DrawGeometry(vb, ib);
}

void MainMenuScene::End()
{
}
