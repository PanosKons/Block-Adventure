#include "MeshParser.h"
#include "Logger.h"

void MeshParser::ParseMesh(std::string&& path, RenderData& renderData)
{
	std::ifstream file(path);
	std::string line;
	std::string garbageData;
	std::string first, second, third, fourth;
	unsigned int counter = 0;
	while (std::getline(file, line))
	{
		Vertex v;
		v.color = { 1.0f,1.0f,1.0f,1.0f };
		v.texCords = { 0.0f,0.0f };
		v.texId = -1;
		if (line[0] == 'v' && line[1] == ' ')
		{
			std::istringstream iss(line);
			iss >> garbageData >> v.position.x >> v.position.y >> v.position.z;
			INFO(v.position.x, ", ", v.position.y, ", ", v.position.z);
			renderData.vertexBuffer.Add(v);
			counter++;
		}
		if (line[0] == 'f' && line[1] == ' ')
		{
			std::istringstream iss(line);
			iss >> garbageData >> first >> second >> third >> fourth;
			INFO(first[0], ", ", second[0], ", ", third[0], ", ", fourth[0]);
			renderData.indexBuffer.AddRectangle(first[0] - '0', second[0] - '0', third[0] - '0', fourth[0] - '0');
		}
	}
	renderData.indexBuffer.AddIndex(counter);
	file.close();
}
