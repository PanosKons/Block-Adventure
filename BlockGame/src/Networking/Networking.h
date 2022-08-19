#pragma once
#include "Common/Networking/Packet.h"
#include "pch.h"
#include "Logger.h"
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
		int BytesSent = send(clientSocket, packet.GetPacket(), packet.GetPacketSize(), 0);
		ASSERT((BytesSent == packet.GetPacketSize()), "Data loss");
	}
	template<int TSize>
	static Packet<TSize> GetPacketFromServer()
	{
		Packet<TSize> packet;
		packet.InitMemory();

		int TotalReceivedBytes = 0;
		do
		{
			int ReceivedBytes = recv(clientSocket, packet.GetPacket() + TotalReceivedBytes, packet.GetPacketSize() - TotalReceivedBytes, 0);
			TotalReceivedBytes += ReceivedBytes;
		} while (TotalReceivedBytes != packet.GetPacketSize());

		return packet;
	}
};

