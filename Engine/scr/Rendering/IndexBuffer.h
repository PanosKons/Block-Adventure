#pragma once
#include "pch.h"
class IndexBuffer
{
public:
	IndexBuffer();
	void AddTriangle();
	void AddRectangle();
	void AddCuboid(unsigned char RenderedSides);
	void AddIndices(std::vector<unsigned int>& indices, int vertex_count);
	void Clear();
	inline std::vector<unsigned int>& GetData() { return m_Indices; };
private:
	std::vector<unsigned int> m_Indices;
	unsigned int m_Index;
};
