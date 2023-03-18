#pragma once
#include "Common/Networking/Packet.h"
#include "pch.h"
#include "Logger.h"
#include "Common/World/Chunk.h"
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

	template<typename T>
	static void SendDataToServer(Packet id,T& data)
	{
		if (id != Packet::None) {
			//Send PacketId
			{
				int TotalSentBytes = 0;
				do
				{
					int SentBytes = send(clientSocket, (char*)&id + TotalSentBytes, sizeof(Packet) - TotalSentBytes, 0);
					TotalSentBytes += SentBytes;
				} while (TotalSentBytes != sizeof(Packet));
			}
		}
		//Send data
		{
			int TotalSentBytes = 0;
			do
			{
				int SentBytes = send(clientSocket, (char*)&data + TotalSentBytes, sizeof(T) - TotalSentBytes, 0);
				TotalSentBytes += SentBytes;
			} while (TotalSentBytes != sizeof(T));
		}
	}
	template<typename T>
	static T GetDataFromServer()
	{
		T data;
		int TotalReceivedBytes = 0;
		do
		{
			int ReceivedBytes = recv(clientSocket, (char*)&data + TotalReceivedBytes, sizeof(T) - TotalReceivedBytes, 0);
			TotalReceivedBytes += ReceivedBytes;
		} while (TotalReceivedBytes != sizeof(T));

		return data;
	}
	static BlockArray* GetChunkDataFromServer()
	{
		BlockArray* data = new BlockArray();
		int TotalReceivedBytes = 0;
		do
		{
			int ReceivedBytes = recv(clientSocket, (char*)data + TotalReceivedBytes, sizeof(BlockArray) - TotalReceivedBytes, 0);
			TotalReceivedBytes += ReceivedBytes;
		} while (TotalReceivedBytes != sizeof(BlockArray));

		return data;
	}
};

