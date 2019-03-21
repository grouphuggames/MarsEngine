#pragma once
#include <Windows.h>
//#include <cstdio>

namespace Mars
{
	void CreateWin32DebugConsole()
	{
		LPCTSTR console_name = "Debug Console";
		AllocConsole();
		SetConsoleTitle(console_name);
		freopen("CONOUT$", "w", stdout);
	}
}
