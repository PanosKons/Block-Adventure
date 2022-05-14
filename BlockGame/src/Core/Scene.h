#pragma once
class Scene
{
public:
	Scene() {};
	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void End() = 0;
};

