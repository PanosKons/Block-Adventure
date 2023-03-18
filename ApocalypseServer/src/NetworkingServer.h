#pragma once
#include "pch.h"
#include "EntityManagerServer.h"
#include "Common/Entities/Player/Player.h"
#include "Common/Math/Vector.h"

namespace NetworkingServer{
	int Send(uint64_t UUID, const char* buffer, int len);
	int Receive(uint64_t UUID, char* buf, int len);

	template<typename T>
	void SendDataToClient(uint64_t UUID, Packet id, T& data)
	{
		if (id != Packet::None) {
			//SendPacketID
			{
				int TotalSentBytes = 0;
				do
				{
					int SentBytes = Send(UUID, (char*)&id + TotalSentBytes, sizeof(Packet) - TotalSentBytes);
					TotalSentBytes += SentBytes;
				} while (TotalSentBytes != sizeof(Packet));
			}
		}
		//SendData
		{
			int TotalSentBytes = 0;
			do
			{
				int SentBytes = Send(UUID, (char*)&data + TotalSentBytes, sizeof(T) - TotalSentBytes);
				TotalSentBytes += SentBytes;
			} while (TotalSentBytes != sizeof(T));
		}

	}
	template<typename T>
	void SendDataAllExceptClient(uint64_t uuid, Packet id, T& data)
	{
		for (auto& [UUID, player] : EntityManagerServer::Players)
		{
			if(UUID != uuid && player->IsReadyToReceivePackets == true)
				SendDataToClient(player->credentials.UUID, id, data);
		}
	}
	template<typename T>
	void SendDataAllClients(Packet id,T& data)
	{
		for (auto&[UUID, player] : EntityManagerServer::Players)
		{
			if(player->IsReadyToReceivePackets == true)
				SendDataToClient(player->credentials.UUID, id, data);
		}
	}
	template<typename T>
	T GetDataFromClient(Credentials& credentials)
	{
		T data;
		int TotalReceivedBytes = 0;
		do
		{
			int ReceivedBytes = Receive(credentials.UUID, (char*)&data + TotalReceivedBytes, sizeof(T) - TotalReceivedBytes);
			TotalReceivedBytes += ReceivedBytes;
		} while (TotalReceivedBytes != sizeof(T));

		return data;
	}
	void ListenForClients();
	void Shutdown();
}