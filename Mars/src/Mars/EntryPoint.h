#pragma once
#include "Surface.h"
#ifdef _DEBUG
#include "Debug.h"
#endif

extern Mars::Application* Mars::CreateApplication();

s32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	SYSTEM_INFO system_info = {};
	s32 core_count = 0;
	GetSystemInfo(&system_info);
	core_count = system_info.dwNumberOfProcessors;

	auto game = Mars::CreateApplication();

#ifdef _DEBUG
	Mars::CreateWin32DebugConsole();
#endif
	Mars::CreateWin32Surface(hInstance, nShowCmd);

	game->Run();
	delete game;

	return 0;
}
