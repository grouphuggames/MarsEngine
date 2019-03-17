#pragma once


#include <Windows.h>
#ifdef ME_BUILD_DLL
    #define MARS_API __declspec(dllexport)
#else
    #define MARS_API __declspec(dllimport)
#endif
