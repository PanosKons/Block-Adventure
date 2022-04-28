#pragma once
#include <vector>
#include <array>
#include "../Math/Vector.h"
class Vertex
{
public:
	Vector3<float> position;
	Vector4<float> color;
	Vector2<float> texCords;
	float texId;
};
class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();
	void Add(Vertex& Element);
	void Allocate();
	void Bind();
	void Clear();
	inline unsigned int GetSize() { return m_Size; }
private:
	unsigned int m_RendererID;
	unsigned int m_Size;
	std::vector<Vertex> m_Vertices;
};
