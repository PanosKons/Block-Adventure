#include "pch.h"
#include "Logger.h"
#include <windows.h>

void Logger::SetConsoleColor(Color color)
{
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)color);
}
