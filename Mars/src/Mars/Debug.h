#pragma once
#include <Windows.h>

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
