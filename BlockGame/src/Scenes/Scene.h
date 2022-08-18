#pragma once
class Scene
{
public:
	Scene() {};
	virtual void Start() = 0;
	virtual void Update(double TimeStep) = 0;
	virtual void Render(double TimeStep) = 0;
	virtual void End() = 0;
};

