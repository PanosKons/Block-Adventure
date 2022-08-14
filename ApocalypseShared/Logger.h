#include "pch.h"
#include <ctime>

#define LOG_ENABLED

#ifdef _DEBUG
#define TRACE(...) Logger::SetConsoleColor(Logger::Gray); Logger::Log(__VA_ARGS__)
#define INFO(...) Logger::SetConsoleColor(Logger::Gray); Logger::Log(__VA_ARGS__)
#define WARN(...) Logger::SetConsoleColor(Logger::Yellow); Logger::Log(__VA_ARGS__)
#define ERR(...) Logger::SetConsoleColor(Logger::Red); Logger::Log(__VA_ARGS__)
#define ASSERT(x,y) if(x==0) { DebugBreak(); }
#elif defined LOG_ENABLED
#define TRACE(...)
#define INFO(...) Logger::SetConsoleColor(Logger::Gray); Logger::Log(__VA_ARGS__)
#define WARN(...) Logger::SetConsoleColor(Logger::Yellow); Logger::Log(__VA_ARGS__)
#define ERR(...) Logger::SetConsoleColor(Logger::Red); Logger::Log(__VA_ARGS__)
#define ASSERT(x,y) if(x==0) { ERR(y); }
#else
#define TRACE(...)
#define INFO(...)
#define WARN(...)
#define ERR(...)
#define ASSERT(x,y)
#endif
class Logger
{
public:
	enum Color
	{
		Green = 10, Red = 12, Yellow = 14, Gray = 7
	};
	template<typename T, typename ...Tpack>
	static void SubLog(T first, Tpack ...pack)
	{

		std::cout << first;
		if constexpr (sizeof...(pack) > 0)
		{
			SubLog(pack...);
		}
		else
		{
			std::cout << "\n";
		}
	}
	template<typename T, typename ...Tpack>
	static void Log(T first, Tpack ...pack)
	{
		std::tm now;
		std::time_t t = std::time(0);
		localtime_s(&now, &t);
		std::cout << "[" << now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec << "] ";
		std::cout << first;
		if constexpr (sizeof...(pack) > 0)
		{
			SubLog(pack...);
		}
		else
		{
			std::cout << "\n";
		}
	}
	static void SetConsoleColor(Color color);
};