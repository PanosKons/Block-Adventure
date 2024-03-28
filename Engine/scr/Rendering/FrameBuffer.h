#pragma once
#include "DepthBuffer.h"
#include "Texture.h"
class FrameBuffer
{
public:
	FrameBuffer(DepthBuffer& depthBuffer, Texture& colorBuffer);
	~FrameBuffer();
	void Bind();
private:
	unsigned int m_RendererID;
};

