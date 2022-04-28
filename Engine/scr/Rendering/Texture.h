#pragma once
#include <string>

class Texture {
private:

public:
	static void Load(const std::string& filepath, unsigned int slot);
	static void Bind(unsigned int slot);
};
