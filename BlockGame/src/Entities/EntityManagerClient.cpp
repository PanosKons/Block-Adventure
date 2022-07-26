#include "EntityManagerClient.h"
#include "Common/Networking/Packet.h"
#include "Networking/Networking.h"

void EntityManagerClient::Tick()
{
	Packet<DefaultPacketSize> packet;
	packet.InitMemory();
	packet.AddPacketData<PACKET_ID>(PACKET_ID::PlayerPosition);
	packet.AddPacketData<Vector3<double>>(Players[Networking::Player_id]->Position);
	Networking::SendPacketToServer(packet);
}
