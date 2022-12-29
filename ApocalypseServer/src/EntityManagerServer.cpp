#include "EntityManagerServer.h"
#include "Reader.h"

void EntityManagerServer::CreatePlayer(Credentials& credentials)
{
	Players[credentials.UUID] = Reader::ReadPlayer(credentials);
}
