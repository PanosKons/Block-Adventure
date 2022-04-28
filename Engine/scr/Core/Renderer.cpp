#include <Engine.h>
#include "Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../vendor/glm/gtc/quaternion.hpp"
#include "../vendor/glm/gtx/quaternion.hpp"
#include "../Rendering/Shader.h"
#include "../Rendering/Texture.h"
#include "../Rendering/VertexBufferLayout.h"
#include "GameManager.h"
#include <iostream>
#include "MainCamera.h"
#include "ManagerUI.h"
#include "Input.h"
#include "GlobalVariables.h"
namespace Renderer {
	static float previous;
	static std::unique_ptr<Shader> m_Shader;
	static std::vector<std::string> m_Textures;
	static glm::mat4 proj;
	static float fov = 70.0f;
	void DrawChunk(Chunk* chunk)
	{
		chunk->m_VertexBuffer->Bind();
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(4);
		layout.Push<float>(2);
		layout.Push<float>(1);
		layout.Calculate();
		glDrawElements(GL_TRIANGLES, (GLsizei)(chunk->m_IndexBuffer->GetData().size()), GL_UNSIGNED_INT, chunk->m_IndexBuffer->GetData().data());
	}
	void DrawGeometry(VertexBuffer& vb, IndexBuffer& ib)
	{
		vb.Bind();
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(4);
		layout.Push<float>(2);
		layout.Push<float>(1);
		layout.Calculate();
		glDrawElements(GL_TRIANGLES, (GLsizei)(ib.GetData().size()), GL_UNSIGNED_INT, ib.GetData().data());
	}
	void DrawGeometry(VertexBuffer& vb, IndexBuffer& ib, unsigned int count, unsigned int offset)
	{
		vb.Bind();
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(4);
		layout.Push<float>(2);
		layout.Push<float>(1);
		layout.Calculate();
		glDrawElements(GL_TRIANGLES, (GLsizei)count, GL_UNSIGNED_INT, ib.GetData().data() + offset);
	}
	void Run()
	{
		while (!glfwWindowShouldClose(ApplicationWindow))
		{
			glm::mat4 view = glm::lookAt(GameManager::player->mainCamera.cameraPos, GameManager::player->mainCamera.cameraPos + GameManager::player->mainCamera.cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
			m_Shader->SetUniformMat4f("u_V", proj * view);
			float now = (float)glfwGetTime();
			float deltaTime = now - previous;
			previous = now;
			glClearColor(0.0f, 0.8f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			GameManager::Update(deltaTime);
			m_Shader->SetUniformMat4f("u_V", glm::ortho(0.0f, (float)ScreenWidth, 0.0f, (float)ScreenHeight, -30.0f, 30.0f));
			ManagerUI::UpdateUI();
			glfwSwapBuffers(ApplicationWindow);
			glfwPollEvents();
		}

		glfwDestroyWindow(ApplicationWindow);
		glfwTerminate();
		GameManager::Shutdown();
	}
	int CreateWindow(const std::string& name)
	{
		std::ios_base::sync_with_stdio(false);
		if (!glfwInit())
			return -1;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		ApplicationWindow = glfwCreateWindow(ScreenWidth, ScreenHeight, name.c_str(), NULL, NULL);
		if (!ApplicationWindow)
		{
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(ApplicationWindow);
		glfwSwapInterval(0);
		glfwSetInputMode(ApplicationWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			return -1;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

		unsigned int m_RendererID;
		glCreateVertexArrays(1, &m_RendererID);
		glBindVertexArray(m_RendererID);

		m_Shader = std::make_unique<Shader>("res/shaders/Base.shader");
		m_Shader->Bind();
		int textures[32] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
		m_Shader->SetUniform1iv("u_texture", textures, 32);
		proj = glm::perspective(glm::radians(fov), (float)ScreenWidth / (float)ScreenHeight, 0.1f, -30.0f);
		Texture::Load("res/textures/cursor.png", 12);
		Texture::Load("res/textures/selected_slot.png", 15);
		Texture::Load("res/textures/text.png", 13);
		Texture::Load("res/textures/slot.png", 14);
		Texture::Load("res/textures/TextureAtlas.png", 0);
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		return 0;
	}
}