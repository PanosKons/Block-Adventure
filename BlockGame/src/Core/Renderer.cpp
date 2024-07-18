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
#include "Entities/Sun.h"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include "vendor/imgui/imgui_impl_glfw.h"


static std::vector<Renderer::RenderCommand> RenderCommandQueue;

static std::unique_ptr<Shader> BaseShader;
static std::unique_ptr<Shader> GuiShader;
static std::unique_ptr<Shader> PostShader;
static std::unique_ptr<Shader> LightShader;
static std::unique_ptr<Shader> LightDepthShader;

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
static unsigned int sBuffer;
static unsigned int sDepth;

static glm::vec4 ortho = glm::vec4(-1280 /16, 1280 /16, -720/16, 720 / 16);
static glm::vec3 ortho2 = glm::vec3(0,0,0);
static glm::vec2 nearfar = glm::vec2(3.0f, 300.0f);
static Vector2<float> shadowResolution = {2000,2000};

void ReloadShaders()
	{
		int textures[32] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
		if (BaseShader->Reload())
		{
			BaseShader->Bind();
			BaseShader->SetUniform1iv("u_texture", textures, 32);
		}
		if (GuiShader->Reload())
		{
			GuiShader->Bind();
			GuiShader->SetUniform1iv("u_texture", textures, 32);
		}
		if (LightShader->Reload())
		{
			LightShader->Bind();			
			LightShader->SetUniform1iv("u_texture", textures, 32);
		}
		if (PostShader->Reload()) {
			PostShader->Bind();
			PostShader->SetUniform1iv("u_texture", textures, 32);
		}
		if (LightDepthShader->Reload()) {
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
	
		//Shadow mapping
		glGenFramebuffers(1, &sBuffer);
		glActiveTexture(GL_TEXTURE0 + 7);
		glGenTextures(1, &sDepth);
		glBindTexture(GL_TEXTURE_2D, sDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			(GLsizei)shadowResolution.x, (GLsizei)shadowResolution.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, sBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, sDepth, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
void Renderer::Init()
{
	ASSERT(glfwInit(), "Glfw failed to initialize");
}
GLFWwindow* Renderer::ConstructWindow(const std::string& name, bool resizeable)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, (int)resizeable);
	GLFWwindow* window = glfwCreateWindow(Client::ScreenWidth, Client::ScreenHeight, name.c_str(), NULL, NULL);
	ASSERT(window, "Failed to construct window");

	BindWindow(window);
	ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initiliaze GLAD");

	return window;
}
void Renderer::Setup()
{
	glfwSwapInterval(1);

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
	LightDepthShader = std::make_unique<Shader>("res/shaders/LightDepth.shader");

	BaseShader->Bind();
	int textures[32] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
	BaseShader->SetUniform1iv("u_texture", textures, 32);
	GuiShader->Bind();
	GuiShader->SetUniform1iv("u_texture", textures, 32);
	LightShader->Bind();
	LightShader->SetUniform1iv("u_texture", textures, 32);
	PostShader->Bind();
	PostShader->SetUniform1iv("u_texture", textures, 32);

	//Slot 3 occupied by post processing
	CursorTexture = std::make_unique<Texture>("res/textures/cursor.png", 12);
	SelectedSlotTexture = std::make_unique<Texture>("res/textures/selected_slot.png", 15);
	TextTexture = std::make_unique<Texture>("res/textures/text.png", 13);
	SlotTexture = std::make_unique<Texture>("res/textures/slot.png", 14);
	SelectionTexture = std::make_unique<Texture>("res/textures/selection.png", 1);
	WorldTexture = std::make_unique<Texture>("res/textures/Robbie.png", 0);

	createFrameBuffer();

	MeshParser::ParseMesh("untitled.obj",0);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}
void Renderer::SetupImGui()
{
	glfwSwapInterval(1);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(Client::ImGuiWindow, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
}
glm::mat4 getPlayerMatrix()
	{
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

		proj = glm::perspective(glm::radians(EntityManagerClient::GetPlayer().Fov), (float)Client::ScreenWidth / (float)Client::ScreenHeight, 0.1f, 300.0f);
		glm::mat4 view = glm::lookAt(CameraPosition, CameraPosition + front, glm::vec3(0.0f, 1.0f, 0.0f));
		return proj * view;
	}
glm::mat4 getSunMatrix()
{
	auto position = Sun::GetPosition();
	auto sunDir = Sun::GetDirection();
	auto playerPos = EntityManagerClient::GetPlayer().Position;
	glm::mat4 lightProjection = glm::ortho(ortho.x, ortho.y, ortho.z, ortho.w, nearfar.x, nearfar.y); //change far_plane, near plane in point lights to render only needed objects
	glm::mat4 lightView = glm::lookAt({ position.x,position.y,position.z }, { (float)floor(playerPos.x),(float)floor(playerPos.y),(float)floor(playerPos.z) }, glm::vec3(0.0f, 1.0f, 0.0f));

	return lightProjection * lightView;
}
void RenderWorld()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		for (Renderer::RenderCommand& renderCommand : RenderCommandQueue)
		{
			if (renderCommand.renderCommandType != Renderer::RenderCommandType::World) continue;
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
void WorldPass()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		BaseShader->Bind();
		BaseShader->SetUniformMat4f("u_V", getPlayerMatrix());
		BaseShader->SetUniformMat4f("lightSpaceMatrix", getSunMatrix());
		RenderWorld();
	}
void PostProcessingPass()
{
	//Post processing
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.4f, 0.6f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	PostShader->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void LightingPass()
{
	auto sunDir = Sun::GetDirection();

	worldFrameBuffer->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	LightShader->Bind();
	
	LightShader->SetUniform3f("lightdir", sunDir.x , sunDir.y, sunDir.z);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE0 + 5);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE0 + 6);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glActiveTexture(GL_TEXTURE0 + 7);
	glBindTexture(GL_TEXTURE_2D, sDepth);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void RenderShadowMap()
{
	LightDepthShader->Bind();
	LightDepthShader->SetUniformMat4f("lightSpaceMatrix", getSunMatrix());
	glViewport(0, 0, (GLsizei)shadowResolution.x, (GLsizei)shadowResolution.y);
	glBindFramebuffer(GL_FRAMEBUFFER, sBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	RenderWorld();
	glViewport(0, 0, Client::ScreenWidth, Client::ScreenHeight);
}
void UIPass()
{
	glDisable(GL_DEPTH_TEST);
	////(UI3D not implemented yet)
	GuiShader->Bind();
	GuiShader->SetUniformMat4f("u_V", glm::ortho(0.0f, (float)Client::ScreenWidth, 0.0f, (float)Client::ScreenHeight, -30.0f, 30.0f));
	//glDepthFunc(GL_LEQUAL);
	for (Renderer::RenderCommand& renderCommand : RenderCommandQueue)
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
void Renderer::Render()
{
	ReloadShaders();

	RenderShadowMap();
	WorldPass();
	LightingPass();
	PostProcessingPass();
	UIPass();
	
	RenderCommandQueue.clear();

	glfwPollEvents();
	glfwSwapBuffers(Client::ApplicationWindow);

	glfwMakeContextCurrent(Client::ImGuiWindow);
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui::NewFrame();

	//ImGui::SetNextWindowSize({ (float)Client::ScreenWidth,(float)Client::ScreenHeight });
	
	ImGui::ShowDemoWindow();
	ImGui::InputFloat4("ortho", (float*)&ortho);
	ImGui::InputFloat3("ortho2", (float*)&ortho2);
	ImGui::InputFloat2("nearfar", (float*)&nearfar);
	ImGui::InputFloat("angle", Sun::getAngle(), 0.01f);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();
	glfwPollEvents();
	glfwSwapBuffers(Client::ImGuiWindow);
	glfwMakeContextCurrent(Client::ApplicationWindow);
}

void Renderer::ShutDown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
		glfwDestroyWindow(Client::ImGuiWindow);
		glfwDestroyWindow(Client::ApplicationWindow);
		glfwTerminate();
	}
bool Renderer::ShouldWindowClose(GLFWwindow* window)
	{
		return glfwWindowShouldClose(window);
	}
void Renderer::AddCommand(RenderCommand& renderCommand)
	{
		RenderCommandQueue.push_back(renderCommand);
	}
void Renderer::HideCursor(bool value)
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
void Renderer::BindWindow(GLFWwindow* window)
{
	glfwMakeContextCurrent(window);
}
