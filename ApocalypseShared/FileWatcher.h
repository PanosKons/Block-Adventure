#pragma once
#include "pch.h"
class FileWatcher
{
public:
	static void WatchFile(std::string&& path, std::function<void()> callback);
};

