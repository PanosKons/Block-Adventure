#include "Texture.h"
#include "../vendor/stb_image.h"
#include "glad/glad.h"

static unsigned char* m_LocalBuffer;
static int m_Width, m_Height, m_BPR;
static unsigned int m_RendererID;
void Texture::Bind(unsigned int slot) {
	/////////m_RendererID is per texture!!! should fix it
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}
void Texture::Load(const std::string& path, unsigned int slot)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPR, 4);
	glActiveTexture(GL_TEXTURE0 + slot);
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	if (m_LocalBuffer) {
		stbi_image_free(m_LocalBuffer);
	}
}