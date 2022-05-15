#pragma once
#include <Engine.h>
#include "Entity.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
class EntityManager
{
public:
	inline static std::vector<Entity*> Entities;
	inline static std::vector<Entity*> Players;
	static void Render();
	static void Start();
	static void ShutDown();
	static void AddPlayer(int id);
	static void UpdatePlayer(int id, Vector3<double> Position);
};

