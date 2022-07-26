#pragma once
#include "Common/Networking/Packet.h"
#include "pch.h"
class Networking
{
public:
	inline static SOCKET clientSocket = 0;
	inline static int Player_id = -1;

	static void Connect();
	static void ShutDown();

	template<int TSize>
	static void SendPacketToServer(Packet<TSize>& packet)
	{
		send(clientSocket, packet.GetPacket(), packet.GetPacketSize(), 0);
	}
	template<int TSize>
	static const Packet<TSize>& GetPacketFromServer()
	{
		Packet<TSize> packet;
		packet.InitMemory();
		recv(clientSocket, packet.GetPacket(), packet.GetPacketSize(), 0);
		return packet;
	}
};

