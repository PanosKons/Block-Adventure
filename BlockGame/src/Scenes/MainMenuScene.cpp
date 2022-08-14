#include "MainMenuScene.h"
#include "Renderer.h"
#include "Client.h"
#include "Core/Input.h"
#include "GameManager.h"
#include "GameScene.h"
#include "Logger.h"
void MainMenuScene::Start()
{
	INFO("MainMenuScene Started");
	RenderBuilder::Begin(renderData);
	RenderBuilder::AddSquare(renderData, { (float)Client::ScreenWidth / 2,(float)Client::ScreenHeight / 2 }, { 200.0f,200.0f }, { 1.0f,1.0f,1.0f,1.0f }, { 0,0 }, { 1,1 }, 15);
	RenderBuilder::End(renderData);
}

void MainMenuScene::Update(float deltaTime)
{
	if (Input::GetMouseState(Mouse::Left) == Action::Press)
	{
		Scene* scene = (Scene*)new GameScene();
		GameManager::SetScene(scene);
	}
}

void MainMenuScene::Render()
{
	Renderer::RenderCommand renderCommand;
	renderCommand.view = Renderer::View::UI;
	renderCommand.Depth = false;
	renderCommand.renderData = &renderData;
	Renderer::AddCommand(renderCommand);
}
void MainMenuScene::End(){}
