#include "EntityManagerServer.h"
#include "Reader.h"

void EntityManagerServer::CreatePlayer(int PlayerId)
{
	Players[PlayerId] = Reader::ReadPlayer(PlayerId);
}
