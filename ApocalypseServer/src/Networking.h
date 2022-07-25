#pragma once
#include "Common/Networking/Packet.h"
namespace Networking{
	void ListenForClients();
	void Shutdown();
	void Tick();
	template<int TSize>
	void SendPacketToClient(unsigned char ClientId, Packet<TSize>& packet)
	{
		send(*sockets[ClientId], packet.GetPacket(), packet.GetPacketSize(), 0);
	}
	template<int TSize>
	void SendAllClients(Packet<TSize>& packet)
	{
		for (int i = 0; i < sockets.size())
		{
			SendPacketToClient(i, packet);
		}
	}
	template<int TSize>
	Packet<TSize> GetPacketFromClient(unsigned char ClientId)
	{
		Packet<TSize> packet;
		packet.InitMemory();
		recv(*sockets(ClientId),packet.GetPacket(),packet.GetPacketSize(),0);
		return packet;
	}
}