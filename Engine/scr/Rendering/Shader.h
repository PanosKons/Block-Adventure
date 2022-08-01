#pragma once
#include "pch.h"
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};
class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_Renderer_ID;
	std::unordered_map < std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();
	void Bind() const;
	void UnBind() const;

	//Set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1iv(const std::string& name, int value[], int count);

private:
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShaders(const std::string& VertexShader, const std::string& FragmentShader);
	int GetUniformLocation(const std::string& name);
	ShaderProgramSource ParseShader(const std::string& filepath);
};
