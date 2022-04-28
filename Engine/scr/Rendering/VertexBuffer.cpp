#include "VertexBuffer.h"
#include "glad/glad.h"
VertexBuffer::VertexBuffer()
	:m_RendererID(0), m_Size(0)
{
	glGenBuffers(1, &m_RendererID);
}
VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}
void VertexBuffer::Clear()
{
	m_Vertices.clear();
}
void VertexBuffer::Allocate()
{
	if (m_Vertices.size() > m_Size)
	{
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_DYNAMIC_DRAW);
		m_Size = (unsigned int)m_Vertices.size();
	}
	else
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_Vertices.size() * sizeof(Vertex), m_Vertices.data());
	}
}
void VertexBuffer::Add(Vertex& Element)
{
	m_Vertices.push_back(Element);
}
void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}