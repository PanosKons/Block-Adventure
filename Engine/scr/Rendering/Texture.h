#pragma once
#include "pch.h"

class Texture {
private:

public:
	static void Load(const std::string& filepath, unsigned int slot);
	static void Bind(unsigned int slot);
};
