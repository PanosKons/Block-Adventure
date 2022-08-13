#pragma once
#include "Common/Networking/Packet.h"
#include "pch.h"
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"WS2_32")


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
	static Packet<TSize> GetPacketFromServer()
	{
		Packet<TSize> packet;
		packet.InitMemory();
		recv(clientSocket, packet.GetPacket(), packet.GetPacketSize(), 0);
		return packet;
	}
};

