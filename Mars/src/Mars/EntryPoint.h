#pragma once
#include "Surface.h"
#include "Debug.h"

extern Mars::Application* Mars::CreateApplication();

s32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	SYSTEM_INFO system = {};
	GetSystemInfo(&system);
	system_info.core_count = system.dwNumberOfProcessors;

	auto game = Mars::CreateApplication();

#ifdef WIN32
	Mars::CreateWin32DebugConsole();
	Mars::CreateWin32Surface(hInstance, nShowCmd);
#endif

	game->Run();

	delete game;
	return 0;
}
