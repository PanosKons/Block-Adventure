#include "IndexBuffer.h"
#include "../Math/EngineMath.h"
IndexBuffer::IndexBuffer()
	:m_Index(0) {}
void IndexBuffer::AddTriangle()
{
	m_Indices.push_back(0 + m_Index);
	m_Indices.push_back(1 + m_Index);
	m_Indices.push_back(2 + m_Index);
	m_Index += 3;
}
void IndexBuffer::AddCuboid(unsigned char RenderedSides)
{
	for (int i = 0; i < 6; i++)
	{
		if ((RenderedSides & (unsigned char)Math::Powi(2, i)) == 0) continue;
		m_Indices.push_back(0 + m_Index);
		m_Indices.push_back(1 + m_Index);
		m_Indices.push_back(2 + m_Index);
		m_Indices.push_back(2 + m_Index);
		m_Indices.push_back(3 + m_Index);
		m_Indices.push_back(0 + m_Index);
		m_Index += 4;
	}
}
void IndexBuffer::AddRectangle()
{
	m_Indices.push_back(0 + m_Index);
	m_Indices.push_back(1 + m_Index);
	m_Indices.push_back(2 + m_Index);
	m_Indices.push_back(2 + m_Index);
	m_Indices.push_back(3 + m_Index);
	m_Indices.push_back(0 + m_Index);
	m_Index += 4;
}
void IndexBuffer::Clear()
{
	m_Indices.clear();
	m_Index = 0;
}