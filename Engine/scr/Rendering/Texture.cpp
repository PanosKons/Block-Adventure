#include "Texture.h"
#include "../vendor/stb_image.h"
#include "glad/glad.h"

void Texture::Bind() {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}
Texture::Texture(const std::string& filepath, unsigned int slot) : m_RendererID(-1), slot(slot)
{
	stbi_set_flip_vertically_on_load(1);
	unsigned char* localBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPR, 4);
	glActiveTexture(GL_TEXTURE0 + slot);
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
	if (localBuffer) {
		stbi_image_free(localBuffer);
	}
}
Texture::Texture(unsigned int slot, int width, int height): m_RendererID(-1), slot(slot), m_Width(width), m_Height(height), m_BPR(-1)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
}
Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}