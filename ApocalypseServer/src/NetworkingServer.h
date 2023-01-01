#pragma once
#include "pch.h"
#include "EntityManagerServer.h"
#include "Common/Entities/Player/Player.h"
namespace NetworkingServer{
	int Send(uint64_t UUID, const char* buffer, int len);
	int Receive(uint64_t UUID, char* buf, int len);
	template<int TSize>
	void SendPacketToClient(Credentials& credentials, Packet<TSize>& packet)
	{
		{
			int TotalSentBytes = 0;
			do
			{
				int SentBytes = Send(credentials.UUID, packet.GetPacket() + TotalSentBytes, packet.GetPacketSize() - TotalSentBytes);
				TotalSentBytes += SentBytes;
			} while (TotalSentBytes != packet.GetPacketSize());
		}
	}
	template<int TSize>
	void SendAllExceptClient(Credentials& credentials, Packet<TSize>& packet)
	{
		for (auto& [UUID, player] : EntityManagerServer::Players)
		{
			if(UUID != credentials.UUID)
				SendPacketToClient(player->credentials, packet);
		}
	}
	template<int TSize>
	void SendAllClients(Packet<TSize>& packet)
	{
		for (auto&[UUID, player] : EntityManagerServer::Players)
		{
			SendPacketToClient(player->credentials, packet);
		}
	}
	template<int TSize>
	Packet<TSize> GetPacketFromClient(Credentials& credentials)
	{
		Packet<TSize> packet;
		packet.InitMemory();
		int TotalReceivedBytes = 0;
		do
		{
			int ReceivedBytes = Receive(credentials.UUID, packet.GetPacket() + TotalReceivedBytes, packet.GetPacketSize() - TotalReceivedBytes);
			TotalReceivedBytes += ReceivedBytes;
		} while (TotalReceivedBytes != packet.GetPacketSize());

		return packet;
	}
	void ListenForClients();
	void Shutdown();
}