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

	RenderBuilder::Begin(m_RenderData);
	RenderBuilder::AddSquare(m_RenderData, { (float)Client::ScreenWidth / 2,(float)Client::ScreenHeight / 2 }, { 200.0f,200.0f }, { 1.0f,1.0f,1.0f,1.0f }, { 0,0 }, { 1,1 }, 15);
	RenderBuilder::End(m_RenderData);
}

void MainMenuScene::Update(double TimeStep)
{
	//if (Input::GetMouseState(Mouse::Left) == MouseState::Click)
	//{
		GameManager::SetScene((Scene*)new GameScene());
	//}
}

void MainMenuScene::Render(double TimeStep)
{
	Renderer::RenderCommand renderCommand;
	renderCommand.view = Renderer::View::UI;
	renderCommand.Depth = false;
	renderCommand.renderData = &m_RenderData;
	Renderer::AddCommand(renderCommand);
}
void MainMenuScene::End(){}
