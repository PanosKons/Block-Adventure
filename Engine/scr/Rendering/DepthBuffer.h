#pragma once
class DepthBuffer
{
public:
	DepthBuffer(int width, int height);
	~DepthBuffer();
	void Bind();
	unsigned int getId() { return m_RendererID; };
private:
	unsigned int m_RendererID;
};