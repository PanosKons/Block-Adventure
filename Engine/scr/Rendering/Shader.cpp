#include "pch.h"
#include "Shader.h"
#include <glad/glad.h>
#include "Logger.h"

Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_Renderer_ID(0), ShaderWatcher(filepath, [this]() {this->SetChanged(); })
{
	ShaderProgramSource source = ParseShader(filepath);
	m_Renderer_ID = CreateShaders(source.VertexSource, source.FragmentSource);
}
Shader::~Shader() {
	glDeleteProgram(m_Renderer_ID);
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return{ ss[0].str(),ss[1].str() };
}
unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << message << " failed to complile shader!" << std::endl;
		return 0;
	}

	return id;
}
unsigned int Shader::CreateShaders(const std::string& VertexShader, const std::string& FragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}
void Shader::Bind() const {
	glUseProgram(m_Renderer_ID);
}
void Shader::UnBind() const {
	glUseProgram(0);
}
bool Shader::Reload()
{
	if (isChanged == true)
	{
		WARN("Shader ", m_FilePath, " modified");
		WARN("Reloading shader...");
		isChanged = false;

		glDeleteProgram(m_Renderer_ID);
		ShaderProgramSource source = ParseShader(m_FilePath);
		m_Renderer_ID = CreateShaders(source.VertexSource, source.FragmentSource);

		return true;
	}
	return false;
}
void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) {
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}
void Shader::SetUniform1i(const std::string& name, int value) {
	glUniform1i(GetUniformLocation(name), value);
}
void Shader::SetUniform1iv(const std::string& name, int value[], int count) {
	glUniform1iv(GetUniformLocation(name), count, value);
}
void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_TRUE, &matrix[0][0]);
}
int Shader::GetUniformLocation(const std::string& name) {
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}
	int location = glGetUniformLocation(m_Renderer_ID, name.c_str());
	if (location == -1) {
		std::cout << "Uniform " << name << " doesnt exist" << std::endl;
	}
	m_UniformLocationCache[name] = location;

	return location;
}