#pragma once
#include "Core.h"
#include <iostream>


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

#define MARS_CORE_INFO(...)	std::cout << "CORE INFO: "; \
								Mars::Log::PrintToLog(__VA_ARGS__);
#define MARS_CORE_INFO_EXTRA(...)	std::cout << "CORE INFO EXTRA: "; \
								Mars::Log::PrintToLog(__VA_ARGS__); \
								(std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
#define MARS_CORE_WARN(...)	std::cout << "CORE WARNING: "; \
								Mars::Log::PrintToLog(__VA_ARGS__); \
								(std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
#define MARS_CORE_ERROR(...)	std::cout << "CORE ERROR: "; \
								Mars::Log::PrintToLog(__VA_ARGS__); \
								(std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
#define MARS_INFO(...)		std::cout << "APP INFO: "; \
								Mars::Log::PrintToLog(__VA_ARGS__);
#define MARS_INFO_EXTRA(...)		std::cout << "APP INFO EXTRA: "; \
								Mars::Log::PrintToLog(__VA_ARGS__); \
								(std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
#define MARS_WARN(...)		std::cout << "APP WARNING: "; \
								Mars::Log::PrintToLog(__VA_ARGS__); \
								(std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
#define MARS_ERROR(...)		std::cout << "APP ERROR: "; \
								Mars::Log::PrintToLog(__VA_ARGS__); \
								(std::cout << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n')
