#pragma once
#include "Serializer.h"
#include <Engine.h>
class Networking
{
public:
	static void Connect();
	static void ShutDown();
	static void SendData(PACKET_ID packet_id, char* data, int sizebytes);
};

