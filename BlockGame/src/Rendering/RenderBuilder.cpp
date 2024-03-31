#include "RenderBuilder.h"
#include "Rendering/MeshParser.h"
#include "Client.h"

void RenderBuilder::Begin(RenderData& renderData)
{
	renderData.vertexBuffer.Clear();
	renderData.indexBuffer.Clear();
}
void RenderBuilder::AddSquare(RenderData& renderData, Vector2<float> Position, Vector2<float> Size, Vector4<float> Color, Vector2<float> TexCords, Vector2<float> TexSize, float TextureID, float layer)
{
	Vertex vertex;
	vertex.color = Color;
	vertex.texId = TextureID;
	vertex.texCords = TexCords;
	vertex.position = { Position.x,Position.y,layer };
	renderData.vertexBuffer.Add(vertex);
	vertex.texCords.y += TexSize.y;
	vertex.position.y += Size.y;
	renderData.vertexBuffer.Add(vertex);
	vertex.texCords.x += TexSize.x;
	vertex.position.x += Size.x;
	renderData.vertexBuffer.Add(vertex);
	vertex.texCords.y = TexCords.y;
	vertex.position.y -= Size.y;
	renderData.vertexBuffer.Add(vertex);
	renderData.indexBuffer.AddRectangle();
}
void RenderBuilder::AddMesh(RenderData& renderData, Vector3<float> Position, Vector3<float> Size, Vector4<float> Color, unsigned int slot)
{
	VertexIndexData& vid = MeshParser::GetMesh(0);

	Vertex vertex;
	vertex.color = { 1.0f,1.0f,1.0f,1.0f };
	vertex.texCords = { 0,0 };
	vertex.texId = -1;

	for (size_t i = 0; i < vid.indices.size(); i += 4)
	{
		vertex.position = Position + vid.vertices[i + 1];
		renderData.vertexBuffer.Add(vertex);
		vertex.position = Position + vid.vertices[i];
		renderData.vertexBuffer.Add(vertex);
		vertex.position = Position + vid.vertices[i + 2];
		renderData.vertexBuffer.Add(vertex);
		vertex.position = Position + vid.vertices[i + 3];
		renderData.vertexBuffer.Add(vertex);
		renderData.indexBuffer.AddRectangle();
	}
}
void RenderBuilder::AddText(RenderData& renderData, std::string_view Text, Vector2<float> Position, float layer)
{
	Vertex vertex;
	vertex.color = { 1,1,1,1 };
	vertex.texId = 13;
	vertex.position = { Position.x,Position.y,layer };
	for (char digit : Text)
	{
		int x = digit % 16;
		int y = digit / 16;
		y++;
		vertex.texCords = { x / 16.0f,1 - (y / 16.0f) };
		renderData.vertexBuffer.Add(vertex);
		vertex.position.y += Client::charHeight;
		vertex.texCords.y += 1 / 16.0f;
		renderData.vertexBuffer.Add(vertex);
		vertex.position.x += Client::charWidth;
		vertex.texCords.x += 1 / 16.0f;
		renderData.vertexBuffer.Add(vertex);
		vertex.position.y -= Client::charHeight;
		vertex.texCords.y -= 1 / 16.0f;
		renderData.vertexBuffer.Add(vertex);
		renderData.indexBuffer.AddRectangle();
		vertex.position.x -= Client::charWidth - Client::charWidthOffset;
	}
}
void RenderBuilder::AddCube(RenderData& renderData, Vector3<float> Position, Vector3<float> Size, Vector4<float> Color)
{
	Vertex vertex;
	vertex.color = Color;
	vertex.texId = -1;
	vertex.texCords = { 0,0 };

	vertex.position = { Position.x,Position.y,Position.z };
	renderData.vertexBuffer.Add(vertex);
	vertex.position.x += Size.x;
	renderData.vertexBuffer.Add(vertex);
	vertex.position.y += Size.y;
	renderData.vertexBuffer.Add(vertex);
	vertex.position.x -= Size.x;
	renderData.vertexBuffer.Add(vertex);
	renderData.indexBuffer.AddRectangle();

	vertex.position = { Position.x,Position.y,Position.z };
	renderData.vertexBuffer.Add(vertex);
	vertex.position.y += Size.y;
	renderData.vertexBuffer.Add(vertex);
	vertex.position.z += Size.z;
	renderData.vertexBuffer.Add(vertex);
	vertex.position.y -= Size.y;
	renderData.vertexBuffer.Add(vertex);
	renderData.indexBuffer.AddRectangle();

	vertex.position = { Position.x,Position.y,Position.z };
	renderData.vertexBuffer.Add(vertex);
	vertex.position.z += Size.z;
	renderData.vertexBuffer.Add(vertex);
	vertex.position.x += Size.x;
	renderData.vertexBuffer.Add(vertex);
	vertex.position.z -= Size.z;
	renderData.vertexBuffer.Add(vertex);
	renderData.indexBuffer.AddRectangle();

	vertex.position = { Position.x ,Position.y,Position.z + Size.z };
	renderData.vertexBuffer.Add(vertex);
	vertex.position.y += Size.y;
	renderData.vertexBuffer.Add(vertex);
	vertex.position.x += Size.x;
	renderData.vertexBuffer.Add(vertex);
	vertex.position.y -= Size.y;
	renderData.vertexBuffer.Add(vertex);
	renderData.indexBuffer.AddRectangle();

	vertex.position = { Position.x + Size.x,Position.y ,Position.z };
	renderData.vertexBuffer.Add(vertex);
	vertex.position.z += Size.z;
	renderData.vertexBuffer.Add(vertex);
	vertex.position.y += Size.y;
	renderData.vertexBuffer.Add(vertex);
	vertex.position.z -= Size.z;
	renderData.vertexBuffer.Add(vertex);
	renderData.indexBuffer.AddRectangle();

	vertex.position = { Position.x,Position.y + Size.y ,Position.z };
	renderData.vertexBuffer.Add(vertex);
	vertex.position.x += Size.x;
	renderData.vertexBuffer.Add(vertex);
	vertex.position.z += Size.z;
	renderData.vertexBuffer.Add(vertex);
	vertex.position.x -= Size.x;
	renderData.vertexBuffer.Add(vertex);
	renderData.indexBuffer.AddRectangle();
}
void RenderBuilder::End(RenderData& renderData)
{
	renderData.vertexBuffer.Bind();
	renderData.vertexBuffer.Allocate();
}
