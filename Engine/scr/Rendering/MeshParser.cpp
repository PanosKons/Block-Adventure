#include "MeshParser.h"
#include "RenderData.h"

void MeshParser::ParseMesh(const std::string& path, unsigned int slot)
{
	VertexIndexData& vid = m_VertexIndexData.emplace_back();

	std::ifstream file(path);
	std::string line;
	std::string garbageData;
	std::string first, second, third, fourth;
	while (std::getline(file, line))
	{
		Vector3<float> position;
		if (line[0] == 'v' && line[1] == ' ')
		{
			std::istringstream iss(line);
			iss >> garbageData >> position.x >> position.y >> position.z;
			vid.vertices.push_back(position);
		}
		if (line[0] == 'f' && line[1] == ' ')
		{
			std::istringstream iss(line);
			iss >> garbageData >> first >> second >> third >> fourth;

			vid.indices.push_back(stoi(first.substr(0,'/')) - 1);
			vid.indices.push_back(stoi(second.substr(0, '/')) - 1);
			vid.indices.push_back(stoi(third.substr(0, '/')) -1 );
			vid.indices.push_back(stoi(fourth.substr(0, '/')) -1);

		}
	}
	file.close();
}

VertexIndexData& MeshParser::GetMesh(unsigned int slot)
{
	return m_VertexIndexData[slot];
}
