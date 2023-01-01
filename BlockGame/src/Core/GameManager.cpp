#include "pch.h"
#include "GameManager.h"
#include "Client.h"
#include "Renderer.h"
#include "Scenes/Scene.h"
#include "Scenes/MainMenuScene.h"
#include "Scenes/GameScene.h"
#include "Networking/NetworkingClient.h"
#undef CreateWindow
static Scene* s_Scene;

void GameManager::Tick(double TimeStep)
{
	s_Scene->Update(TimeStep);
	s_Scene->Render(TimeStep);
	Renderer::Render();
}
void GameManager::Start()
{
	//Manual ip input
	//std::wcin >> Client::ip;
	//std::cin >> Client::port;
	//Automatic ip input
	{
		GameName Name;
		std::string in;
		std::cin >> in;
		strcpy_s(Name.data(),Name.size()-1, in.c_str());
		NetworkingClient::credentials = new Credentials(Name, Credentials::ConvertToUUID(Name));
	}
	Client::ip = L"127.0.0.1";
	Client::port = 25555;
	Client::ScreenWidth = 1920 / 2;
	Client::ScreenHeight = 1080 / 2;

	Renderer::CreateWindow("GAME");

	s_Scene = (Scene*)new MainMenuScene();
	s_Scene->Start();
}
void GameManager::SetScene(Scene* scene)
{
	if (s_Scene != scene)
	{
		s_Scene->End();
		delete s_Scene;
		s_Scene = scene;
		s_Scene->Start();
	}
}
void GameManager::Shutdown()
{
	s_Scene->End();
	delete s_Scene;
}