#pragma once
#include "Common/Networking/Packet.h"
#include "pch.h"
#include "Logger.h"
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"WS2_32")


class NetworkingClient
{
public:
	inline static Credentials* credentials;
	inline static SOCKET clientSocket = 0;

	static void Connect();
	static void ShutDown();

	template<int TSize>
	static void SendPacketToServer(Packet<TSize>& packet)
	{
		{
			int TotalSentBytes = 0;
			do
			{
				int SentBytes = send(clientSocket, packet.GetPacket() + TotalSentBytes, packet.GetPacketSize() - TotalSentBytes, 0);
				TotalSentBytes += SentBytes;
			} while (TotalSentBytes != packet.GetPacketSize());
		}
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

