#pragma once
class ServerManager
{
public:
	static void Start();
	static void Tick(double TimeStep);
	static void Shutdown();
};

