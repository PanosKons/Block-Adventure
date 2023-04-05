#pragma once
#include "pch.h"
#include "Common/Math/Vector.h"

struct VertexIndexData
{
	std::vector<Vector3<float>> vertices;
	std::vector<unsigned int> indices;
};
class MeshParser
{
public:
	static void ParseMesh(const std::string& path, unsigned int slot);
	static VertexIndexData& GetMesh(unsigned int slot);
private:
	inline static std::vector<VertexIndexData> m_VertexIndexData;
};

