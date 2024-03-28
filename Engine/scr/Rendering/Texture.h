#pragma once
#include "pch.h"

class Texture {
public:
	Texture(unsigned int slot, int width, int height);
	Texture(const std::string& filepath, unsigned int slot);
	~Texture();
	void Bind();
	unsigned int getId() { return m_RendererID; };
private:
	unsigned int m_RendererID;
	unsigned int slot;
	int m_Width, m_Height, m_BPR;
};
