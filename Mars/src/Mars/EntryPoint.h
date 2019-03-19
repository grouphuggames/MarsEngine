#pragma once
#include "Log.h"


#include "Surface.h"
#ifdef _DEBUG
#include "Debug.h"
#endif

extern Mars::Application* Mars::CreateApplication();

int32_t WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	auto game = Mars::CreateApplication();

#ifdef _DEBUG
	CreateWin32DebugConsole();
#endif
	CreateWin32Surface(hInstance, nShowCmd);

	SYSTEM_INFO system_info = {};
	int32_t core_count = 0;
	GetSystemInfo(&system_info);
	core_count = system_info.dwNumberOfProcessors;

	MARS_CORE_INFO("Systems Setup OK!");

	game->Run();
	delete game;

	return 0;
}
