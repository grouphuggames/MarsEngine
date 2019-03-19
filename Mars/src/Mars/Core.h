#pragma once


#include <cstdint>
#include <iostream>
#include <Windows.h>
#ifdef ME_BUILD_DLL
    #define MARS_API __declspec(dllexport)
#else
    #define MARS_API __declspec(dllimport)
#endif
