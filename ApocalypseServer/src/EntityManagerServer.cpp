#include "EntityManagerServer.h"
#include "Reader.h"

void EntityManagerServer::CreatePlayer(Credentials& credentials)
{
	std::mutex lock;
	lock.lock();
	Players[credentials.UUID] = Reader::ReadPlayer(credentials);
	lock.unlock();
}
