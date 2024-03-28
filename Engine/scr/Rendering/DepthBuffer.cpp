#include "DepthBuffer.h"
#include "glad/glad.h"

DepthBuffer::DepthBuffer(int width, int height)
{
	glGenRenderbuffers(1, &m_RendererID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

DepthBuffer::~DepthBuffer()
{
	glDeleteRenderbuffers(1, &m_RendererID);
}
void DepthBuffer::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
}
