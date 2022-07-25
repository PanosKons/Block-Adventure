#pragma once
#include <string>
#include <sstream>

class StringConvertions
{
public:
	template<typename T>
	static std::string ToString(T value)
	{
		std::ostringstream os;

		os << value;
		return os.str();
	}
};
