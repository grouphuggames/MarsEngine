#pragma once


#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif
#include <cstdint>

#ifdef _WIN32
    #ifdef ME_BUILD_DLL
        #define MARS_API __declspec(dllexport)
    #else
        #define MARS_API __declspec(dllimport)
    #endif
#else
    #ifdef ME_BUILD_DLL
        #define MARS_API __attribute__((visibility("default")))
    #else
        #define MARS_API
    #endif
#endif
    
#ifdef _WIN32
    #define MARS_INLINE __forceinline
#else
    #define MARS_INLINE inline
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
    private:
        static void PrintToLog(){}
        
	public:

		template<typename T, typename... Types>
		static void PrintToLog(T var1, Types... var2)
		{
			std::cout << var1 << "\n";
            PrintToLog(var2...);
		}
	};
}

#ifdef _WIN32
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
#else
#define MARS_CORE_INFO(...)         std::cout << "CORE INFO: "; \
								    Mars::Log::PrintToLog(__VA_ARGS__);
#define MARS_CORE_INFO_EXTRA(...)   std::cout << "CORE INFO EXTRA: "; \
                                    Mars::Log::PrintToLog(__VA_ARGS__); \
								    (std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
#define MARS_CORE_WARN(...)         std::cout << "CORE WARNING: "; \
								    Mars::Log::PrintToLog(__VA_ARGS__); \
								    (std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
#define MARS_CORE_ERROR(...)        std::cout << "CORE ERROR: "; \
								    Mars::Log::PrintToLog(__VA_ARGS__); \
								    (std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
#define MARS_INFO(...)              std::cout << "APP INFO: "; \
								    Mars::Log::PrintToLog(__VA_ARGS__);
#define MARS_INFO_EXTRA(...)        std::cout << "APP INFO EXTRA: "; \
								    Mars::Log::PrintToLog(__VA_ARGS__); \
								    (std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
#define MARS_WARN(...)              std::cout << "APP WARNING: "; \
								    Mars::Log::PrintToLog(__VA_ARGS__); \
								    (std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
#define MARS_ERROR(...)             std::cout << "APP ERROR: "; \
								    Mars::Log::PrintToLog(__VA_ARGS__); \
								    (std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
#endif


// adding MARS_API here fixed issue with having to add game_state.hwnd = hwnd to Application.cpp
MARS_API struct
{
public:
	const char* window_title = "GameWindow";
	s32 window_width = 0;
	s32 window_height = 0;
	f32 elapsed_time = 0.f;
	f32 current_frame_time = 0.f;
	f32 last_frame_time = 0.f;
	bool running = true;
	bool fullscreen = false;
#ifdef _WIN32
	HWND hwnd = NULL;
	HGLRC render_context = NULL;
#endif
} game_state;

MARS_API struct
{
	s32 core_count;
} system_info;
