#include "pch.h"
#include "Credentials.h"
std::ostream& operator<<(std::ostream& os, GameName const& m) {
	for (size_t i = 0; i < m.size(); i++)
	{
		os << m[i];
	}
	return os;
}