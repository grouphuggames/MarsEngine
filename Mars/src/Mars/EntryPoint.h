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

	Mars::CreateWin32DebugConsole();
	Mars::CreateWin32Surface(hInstance, nShowCmd);

	HWND tmp_hwnd = game_state.hwnd;
	game->Run(tmp_hwnd);

	delete game;
	return 0;
}
