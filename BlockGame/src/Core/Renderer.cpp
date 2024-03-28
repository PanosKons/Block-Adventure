#include "Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vendor/glm/gtc/quaternion.hpp"
#include "vendor/glm/gtx/quaternion.hpp"
#include "Rendering/Shader.h"
#include "Rendering/FrameBuffer.h"
#include "Rendering/VertexBufferLayout.h"
#include "Rendering/MeshParser.h"
#include "GameManager.h"
#include "Client.h"
#include "Entities/EntityManagerClient.h"

namespace Renderer {

	static std::vector<RenderCommand> RenderCommandQueue;

	static std::unique_ptr<Shader> BaseShader;
	static std::unique_ptr<Shader> GuiShader;
	static std::unique_ptr<Shader> PostShader;
	static std::unique_ptr<Shader> LightShader;

	static glm::mat4 proj;

	static std::unique_ptr<Texture> CursorTexture;
	static std::unique_ptr<Texture> SelectedSlotTexture;
	static std::unique_ptr<Texture> TextTexture;
	static std::unique_ptr<Texture> SlotTexture;
	static std::unique_ptr<Texture> SelectionTexture;
	static std::unique_ptr<Texture> WorldTexture;


	static std::unique_ptr<Texture> colorBuffer;
	static std::unique_ptr<DepthBuffer> depthBuffer;
	static std::unique_ptr<FrameBuffer> worldFrameBuffer;

	static unsigned int gBuffer;
	static unsigned int gPosition, gNormal, gAlbedoSpec, gDepth;

	void ReloadShaders()
	{
		if (BaseShader->Reload())
		{
			BaseShader->Bind();
			int textures[32] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
			BaseShader->SetUniform1iv("u_texture", textures, 32);
		}
		if (GuiShader->Reload())
		{
			GuiShader->Bind();
			int textures[32] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
			GuiShader->SetUniform1iv("u_texture", textures, 32);
		}
		if (LightShader->Reload())
		{
			LightShader->Bind();
			int textures[32] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
			LightShader->SetUniform1iv("u_texture", textures, 32);
		}
		if (PostShader->Reload()) {
			PostShader->Bind();
			PostShader->SetUniform1i("colorBuffer", 3);
		}
	}

	void createFrameBuffer()
	{
		glGenFramebuffers(1, &gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

		// - position color buffer
		glActiveTexture(GL_TEXTURE0 + 4);
		glGenTextures(1, &gPosition);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Client::ScreenWidth, Client::ScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

		// - normal color buffer
		glActiveTexture(GL_TEXTURE0 + 5);
		glGenTextures(1, &gNormal);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Client::ScreenWidth, Client::ScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

		// - color + specular color buffer
		glActiveTexture(GL_TEXTURE0 + 6);
		glGenTextures(1, &gAlbedoSpec);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Client::ScreenWidth, Client::ScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

		// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);

		glGenRenderbuffers(1, &gDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, gDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Client::ScreenWidth, Client::ScreenHeight);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gDepth);
	}

	int CreateWindow(const std::string& name)
	{
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

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			return -1;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

		unsigned int m_RendererID;
		glCreateVertexArrays(1, &m_RendererID);
		glBindVertexArray(m_RendererID);
		
		colorBuffer = std::make_unique<Texture>(3, Client::ScreenWidth, Client::ScreenHeight);
		depthBuffer = std::make_unique<DepthBuffer>(Client::ScreenWidth, Client::ScreenHeight);
		worldFrameBuffer = std::make_unique<FrameBuffer>(*depthBuffer.get(), *colorBuffer.get());

		BaseShader = std::make_unique<Shader>("res/shaders/World.shader");
		PostShader = std::make_unique<Shader>("res/shaders/Post.shader");
		GuiShader = std::make_unique<Shader>("res/shaders/Gui.shader");
		LightShader = std::make_unique<Shader>("res/shaders/Light.shader");

		BaseShader->Bind();
		int textures[32] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
		BaseShader->SetUniform1iv("u_texture", textures, 32);
		GuiShader->Bind();
		GuiShader->SetUniform1iv("u_texture", textures, 32);
		LightShader->Bind();
		LightShader->SetUniform1iv("u_texture", textures, 32);

		//Slot 3 occupied by post processing
		CursorTexture = std::make_unique<Texture>("res/textures/cursor.png", 12);
		SelectedSlotTexture = std::make_unique<Texture>("res/textures/selected_slot.png", 15);
		TextTexture = std::make_unique<Texture>("res/textures/text.png", 13);
		SlotTexture = std::make_unique<Texture>("res/textures/slot.png", 14);
		SelectionTexture = std::make_unique<Texture>("res/textures/selection.png", 1);
		WorldTexture = std::make_unique<Texture>("res/textures/Robbie.png", 0);
		PostShader->Bind();
		PostShader->SetUniform1i("colorBuffer", 3);

		createFrameBuffer();

		MeshParser::ParseMesh("untitled.obj",0);
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		return 0;
	}

	void WorldRenderPass()
	{
		//glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Vector3<double> LookPosition = EntityManagerClient::GetPlayer().GetLookPosition();
		glm::vec3 CameraPosition = { LookPosition.x,LookPosition.y,LookPosition.z };

		glm::vec3 front;
		if (EntityManagerClient::GetPlayer().cameraMode == CameraMode::ThirdPersonFront)
		{
			front.x = -cos(glm::radians(EntityManagerClient::GetPlayer().Yaw)) * cos(glm::radians(EntityManagerClient::GetPlayer().Pitch));
			front.y = -sin(glm::radians(EntityManagerClient::GetPlayer().Pitch));
			front.z = -sin(glm::radians(EntityManagerClient::GetPlayer().Yaw)) * cos(glm::radians(EntityManagerClient::GetPlayer().Pitch));
		}
		else
		{
			front.x = cos(glm::radians(EntityManagerClient::GetPlayer().Yaw)) * cos(glm::radians(EntityManagerClient::GetPlayer().Pitch));
			front.y = sin(glm::radians(EntityManagerClient::GetPlayer().Pitch));
			front.z = sin(glm::radians(EntityManagerClient::GetPlayer().Yaw)) * cos(glm::radians(EntityManagerClient::GetPlayer().Pitch));
		}
		glm::normalize(front);

		proj = glm::perspective(glm::radians(EntityManagerClient::GetPlayer().Fov), (float)Client::ScreenWidth / (float)Client::ScreenHeight, 0.1f, -30.0f);
		glm::mat4 view = glm::lookAt(CameraPosition, CameraPosition + front, glm::vec3(0.0f, 1.0f, 0.0f));
		BaseShader->Bind();
		BaseShader->SetUniformMat4f("u_V", proj * view);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		for (RenderCommand& renderCommand : RenderCommandQueue)
		{
			if (renderCommand.renderCommandType != RenderCommandType::World) continue;
			renderCommand.renderData->vertexBuffer.Bind();
			VertexBufferLayout layout;
			layout.Push<float>(3);
			layout.Push<float>(4);
			layout.Push<float>(2);
			layout.Push<float>(1);
			layout.Push<float>(3);
			layout.Calculate();

			glDrawElements(GL_TRIANGLES, (GLsizei)(renderCommand.renderData->indexBuffer.GetData().size()), GL_UNSIGNED_INT, renderCommand.renderData->indexBuffer.GetData().data());
		}
	}
	void UIRenderPass()
	{
		glDisable(GL_DEPTH_TEST);
		////(UI3D not implemented yet)
		GuiShader->Bind();
		GuiShader->SetUniformMat4f("u_V", glm::ortho(0.0f, (float)Client::ScreenWidth, 0.0f, (float)Client::ScreenHeight, -30.0f, 30.0f));
		//glDepthFunc(GL_LEQUAL);
		for (RenderCommand& renderCommand : RenderCommandQueue)
		{
			renderCommand.renderData->vertexBuffer.Bind();
			VertexBufferLayout layout;
			layout.Push<float>(3);
			layout.Push<float>(4);
			layout.Push<float>(2);
			layout.Push<float>(1);
			layout.Push<float>(3);
			layout.Calculate();
			glDrawElements(GL_TRIANGLES, (GLsizei)(renderCommand.renderData->indexBuffer.GetData().size()), GL_UNSIGNED_INT, renderCommand.renderData->indexBuffer.GetData().data());
		}
	}
	float angle = 0.0f;
	void LightingPass()
	{
		//////
		angle += 0.01f;
		////// 
		//glDisable(GL_DEPTH_TEST);/////
		worldFrameBuffer->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		LightShader->Bind();
		LightShader->SetUniform3f("lightdir", cos(angle), sin(angle), 0);
		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE0 + 6);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Post processing
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.4f, 0.6f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		PostShader->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void Render()
	{
		//Shader reloading from file
		ReloadShaders();

		WorldRenderPass();
		LightingPass();
		UIRenderPass();
		
		RenderCommandQueue.clear();
		glfwSwapBuffers(Client::ApplicationWindow);
		glfwPollEvents();
	}

	void ShutDown()
	{
		glfwDestroyWindow(Client::ApplicationWindow);
		glfwTerminate();
	}
	bool ShouldWindowClose()
	{
		return glfwWindowShouldClose(Client::ApplicationWindow);
	}
	void AddCommand(RenderCommand& renderCommand)
	{
		RenderCommandQueue.push_back(renderCommand);
	}
	void HideCursor(bool value)
	{
		static bool prev = false;
		if (value == true && prev == false)
		{
			glfwSetInputMode(Client::ApplicationWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			prev = true;
		}
		else if(value == false && prev == true)
		{
			glfwSetInputMode(Client::ApplicationWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			prev = false;
		}
	}
}
namespace RenderBuilder {
	void Begin(RenderData& renderData)
	{
		renderData.vertexBuffer.Clear();
		renderData.indexBuffer.Clear();
	}
	void AddSquare(RenderData& renderData, Vector2<float> Position, Vector2<float> Size, Vector4<float> Color, Vector2<float> TexCords, Vector2<float> TexSize, float TextureID, float layer)
	{
		Vertex vertex;
		vertex.color = Color;
		vertex.texId = TextureID;
		vertex.texCords = TexCords;
		vertex.position = { Position.x,Position.y,layer };
		renderData.vertexBuffer.Add(vertex);
		vertex.texCords.y += TexSize.y;
		vertex.position.y += Size.y;
		renderData.vertexBuffer.Add(vertex);
		vertex.texCords.x += TexSize.x;
		vertex.position.x += Size.x;
		renderData.vertexBuffer.Add(vertex);
		vertex.texCords.y = TexCords.y;
		vertex.position.y -= Size.y;
		renderData.vertexBuffer.Add(vertex);
		renderData.indexBuffer.AddRectangle();
	}
	void AddMesh(RenderData& renderData, Vector3<float> Position, Vector3<float> Size, Vector4<float> Color, unsigned int slot)
	{
		VertexIndexData& vid = MeshParser::GetMesh(0);

		Vertex vertex;
		vertex.color = {1.0f,1.0f,1.0f,1.0f};
		vertex.texCords = { 0,0 };
		vertex.texId = -1;

		for (size_t i = 0; i < vid.indices.size(); i += 4)
		{
			vertex.position = Position + vid.vertices[i+1];
			renderData.vertexBuffer.Add(vertex);
			vertex.position = Position + vid.vertices[i];
			renderData.vertexBuffer.Add(vertex);
			vertex.position = Position + vid.vertices[i+2];
			renderData.vertexBuffer.Add(vertex);
			vertex.position = Position + vid.vertices[i+3];
			renderData.vertexBuffer.Add(vertex);
			renderData.indexBuffer.AddRectangle();
		}
	}
	void AddText(RenderData& renderData, std::string_view Text, Vector2<float> Position, float layer)
	{
		Vertex vertex;
		vertex.color = { 1,1,1,1 };
		vertex.texId = 13;
		vertex.position = { Position.x,Position.y,layer};
		for (char digit : Text)
		{
			int x = digit % 16;
			int y = digit / 16;
			y++;
			vertex.texCords = { x / 16.0f,1 - (y / 16.0f) };
			renderData.vertexBuffer.Add(vertex);
			vertex.position.y += Client::charHeight;
			vertex.texCords.y += 1 / 16.0f;
			renderData.vertexBuffer.Add(vertex);
			vertex.position.x += Client::charWidth;
			vertex.texCords.x += 1 / 16.0f;
			renderData.vertexBuffer.Add(vertex);
			vertex.position.y -= Client::charHeight;
			vertex.texCords.y -= 1 / 16.0f;
			renderData.vertexBuffer.Add(vertex);
			renderData.indexBuffer.AddRectangle();
			vertex.position.x -= Client::charWidth - Client::charWidthOffset;
		}
	}
	void AddCube(RenderData& renderData, Vector3<float> Position, Vector3<float> Size, Vector4<float> Color)
	{
		Vertex vertex;
		vertex.color = Color;
		vertex.texId = -1;
		vertex.texCords = {0,0};

		vertex.position = { Position.x,Position.y,Position.z };
		renderData.vertexBuffer.Add(vertex);
		vertex.position.x += Size.x;
		renderData.vertexBuffer.Add(vertex);
		vertex.position.y += Size.y;
		renderData.vertexBuffer.Add(vertex);
		vertex.position.x -= Size.x;
		renderData.vertexBuffer.Add(vertex);
		renderData.indexBuffer.AddRectangle();

		vertex.position = { Position.x,Position.y,Position.z };
		renderData.vertexBuffer.Add(vertex);
		vertex.position.y += Size.y;
		renderData.vertexBuffer.Add(vertex);
		vertex.position.z += Size.z;
		renderData.vertexBuffer.Add(vertex);
		vertex.position.y -= Size.y;
		renderData.vertexBuffer.Add(vertex);
		renderData.indexBuffer.AddRectangle();

		vertex.position = { Position.x,Position.y,Position.z };
		renderData.vertexBuffer.Add(vertex);
		vertex.position.z += Size.z;
		renderData.vertexBuffer.Add(vertex);
		vertex.position.x += Size.x;
		renderData.vertexBuffer.Add(vertex);
		vertex.position.z -= Size.z;
		renderData.vertexBuffer.Add(vertex);
		renderData.indexBuffer.AddRectangle();

		vertex.position = { Position.x ,Position.y,Position.z + Size.z };
		renderData.vertexBuffer.Add(vertex);
		vertex.position.y += Size.y;
		renderData.vertexBuffer.Add(vertex);
		vertex.position.x += Size.x;
		renderData.vertexBuffer.Add(vertex);
		vertex.position.y -= Size.y;
		renderData.vertexBuffer.Add(vertex);
		renderData.indexBuffer.AddRectangle();

		vertex.position = { Position.x + Size.x,Position.y ,Position.z };
		renderData.vertexBuffer.Add(vertex);
		vertex.position.z += Size.z;
		renderData.vertexBuffer.Add(vertex);
		vertex.position.y += Size.y;
		renderData.vertexBuffer.Add(vertex);
		vertex.position.z -= Size.z;
		renderData.vertexBuffer.Add(vertex);
		renderData.indexBuffer.AddRectangle();

		vertex.position = { Position.x,Position.y + Size.y ,Position.z };
		renderData.vertexBuffer.Add(vertex);
		vertex.position.x += Size.x;
		renderData.vertexBuffer.Add(vertex);
		vertex.position.z += Size.z;
		renderData.vertexBuffer.Add(vertex);
		vertex.position.x -= Size.x;
		renderData.vertexBuffer.Add(vertex);
		renderData.indexBuffer.AddRectangle();
	}
	void End(RenderData& renderData)
	{
		renderData.vertexBuffer.Bind();
		renderData.vertexBuffer.Allocate();
	}
};