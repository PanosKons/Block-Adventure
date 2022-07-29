#include "Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vendor/glm/gtc/quaternion.hpp"
#include "vendor/glm/gtx/quaternion.hpp"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/VertexBufferLayout.h"
#include "GameManager.h"
#include "Entities/MainCamera.h"
#include "UI/ManagerUI.h"
#include "Client.h"
#include "Entities/EntityManagerClient.h"

namespace Renderer {

	static std::vector<RenderCommand> RenderCommandQueue;

	static View view = View::None;
	static std::unique_ptr<Shader> m_Shader;
	static std::vector<std::string> m_Textures;
	static glm::mat4 proj;

	int CreateWindow(const std::string& name)
	{
		std::ios_base::sync_with_stdio(false);

		if (!glfwInit())
			return -1;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		Client::ApplicationWindow = glfwCreateWindow(Client::ScreenWidth, Client::ScreenHeight, name.c_str(), NULL, NULL);

		if (!Client::ApplicationWindow)
		{
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(Client::ApplicationWindow);
		glfwSwapInterval(1);
		glfwSetInputMode(Client::ApplicationWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			return -1;

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
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
	void Run()
	{
		while (!glfwWindowShouldClose(Client::ApplicationWindow))
		{
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (RenderCommand& renderCommand : RenderCommandQueue)
			{
				if (view != renderCommand.view)
				{
					view = renderCommand.view;
					if (view == View::Player)
					{
						proj = glm::perspective(glm::radians(EntityManagerClient::GetPlayer().Fov), (float)Client::ScreenWidth / (float)Client::ScreenHeight, 0.1f, -30.0f);
						glm::mat4 view = glm::lookAt(EntityManagerClient::GetPlayer().GetCameraPosition(), EntityManagerClient::GetPlayer().GetCameraPosition() + EntityManagerClient::GetPlayer().GetCameraFront(), glm::vec3(0.0f, 1.0f, 0.0f));
						m_Shader->SetUniformMat4f("u_V", proj * view);
					}
					else if (view == View::UI)
					{
						m_Shader->SetUniformMat4f("u_V", glm::ortho(0.0f, (float)Client::ScreenWidth, 0.0f, (float)Client::ScreenHeight, -30.0f, 30.0f));
					}
				}
				renderCommand.renderData->vertexBuffer.Bind();
				VertexBufferLayout layout;
				layout.Push<float>(3);
				layout.Push<float>(4);
				layout.Push<float>(2);
				layout.Push<float>(1);
				layout.Calculate();
				if (renderCommand.Depth == true)
				{
					glDepthFunc(GL_ALWAYS);
					glDrawElements(GL_TRIANGLES, (GLsizei)(renderCommand.renderData.indexBuffer.GetData().size()), GL_UNSIGNED_INT, renderCommand.renderData.indexBuffer.GetData().data());
					glDepthFunc(GL_LEQUAL);
				}
				else
				{
					glDrawElements(GL_TRIANGLES, (GLsizei)(renderCommand.renderData.indexBuffer.GetData().size()), GL_UNSIGNED_INT, renderCommand.renderData.indexBuffer.GetData().data());
				}
			}
			RenderCommandQueue.empty();
			glfwSwapBuffers(Client::ApplicationWindow);
			glfwPollEvents();
		}

		glfwDestroyWindow(Client::ApplicationWindow);
		glfwTerminate();
	}

	/*
	void DrawText(VertexBuffer& vb, IndexBuffer& ib, std::string Text, Vector3<float> position)
	{
		Vertex a;
		a.color = { 1,1,1,1 };
		a.texId = 13;
		a.position = position;
		for (char digit : Text)
		{
			int x = digit % 16;
			int y = digit / 16;
			y++;
			a.texCords = { x / 16.0f,1 - (y / 16.0f) };
			vb.Add(a);
			a.position.y += charHeight;
			a.texCords.y += 1 / 16.0f;
			vb.Add(a);
			a.position.x += charWidth;
			a.texCords.x += 1 / 16.0f;
			vb.Add(a);
			a.position.y -= charHeight;
			a.texCords.y -= 1 / 16.0f;
			vb.Add(a);
			ib.AddRectangle();
			a.position.x -= charWidth - charWidthOffset;
		}
	}
	void DrawSquare(VertexBuffer& vb, IndexBuffer& ib, Vector3<float> Position, Vector2<float> Size, Vector4<float> Color, Vector2<float> TexCords, Vector2<float> TexSize, float TextureID)
	{
		Vertex a;
		a.color = Color;
		a.texId = TextureID;
		a.texCords = TexCords;
		a.position = Position;
		vb.Add(a);
		a.texCords.y += TexSize.y;
		a.position.y += Size.y;
		vb.Add(a);
		a.texCords.x += TexSize.x;
		a.position.x += Size.x;
		vb.Add(a);
		a.texCords.y = TexCords.y;
		a.position.y -= Size.y;
		vb.Add(a);
		ib.AddRectangle();
	}
	*/
}