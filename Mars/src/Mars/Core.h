#pragma once


#include <iostream>
#include <Windows.h>
#include <cstdint>


#ifdef ME_BUILD_DLL
    #define MARS_API __declspec(dllexport)
#else
    #define MARS_API __declspec(dllimport)
#endif


using u8 = uint8_t;
using s32 = int32_t;
using u32 = uint32_t;
using s64 = int64_t;
using u64 = uint64_t;
using f32 = float;

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


// adding MARS_API here fixed issue with having to add game_state.hwnd = hwnd to Application.cpp
MARS_API struct
{
public:
	const char* window_title = "GameWindow";
	s32 width;
	s32 height;
	f32 framerate = 0.f;
	bool running = true;
	bool fullscreen = false;
	HWND hwnd = NULL;
} game_state;

struct
{
	s32 core_count;
} system_info;
