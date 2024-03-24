#pragma once
#include "pch.h"
class FileWatcher
{
public:
	FileWatcher(const std::string& path, std::function<void()> callback);
	~FileWatcher();
private:
	void* watch;
};

