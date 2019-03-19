#pragma once
#include "Core.h"


namespace Mars
{
	class MARS_API Log
	{
	public:

		template<typename... T>
		static void PrintToLog(T&&... t)
		{
			(std::cout << ... << t) << '\n';
		}
	};
}

#define MARS_CORE_INFO(...)		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN); \
								std::cout << "CORE INFO: "; \
								Mars::Log::PrintToLog(__VA_ARGS__);
#define MARS_CORE_INFO_EXTRA(...)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN); \
								std::cout << "CORE INFO EXTRA: "; \
								Mars::Log::PrintToLog(__VA_ARGS__); \
								(std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
#define MARS_CORE_WARN(...)		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE); \
								std::cout << "CORE WARNING: "; \
								Mars::Log::PrintToLog(__VA_ARGS__); \
								(std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
#define MARS_CORE_ERROR(...)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED); \
								std::cout << "CORE ERROR: "; \
								Mars::Log::PrintToLog(__VA_ARGS__); \
								(std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
#define MARS_INFO(...)			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN); \
								std::cout << "APP INFO: "; \
								Mars::Log::PrintToLog(__VA_ARGS__);
#define MARS_INFO_EXTRA(...)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN); \
								std::cout << "APP INFO EXTRA: "; \
								Mars::Log::PrintToLog(__VA_ARGS__); \
								(std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
#define MARS_WARN(...)			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE); \
								std::cout << "APP WARNING: "; \
								Mars::Log::PrintToLog(__VA_ARGS__); \
								(std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
#define MARS_ERROR(...)			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED); \
								std::cout << "APP ERROR: "; \
								Mars::Log::PrintToLog(__VA_ARGS__); \
								(std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
