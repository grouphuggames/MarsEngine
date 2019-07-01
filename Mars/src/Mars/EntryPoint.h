#pragma once
#include "Surface.h"
#include "Debug.h"
#include <thread>

extern Mars::Application* Mars::CreateApplication();

#ifdef _WIN32
s32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	system_info.core_count = std::thread::hardware_concurrency();

	auto game = Mars::CreateApplication();

	Mars::CreateWin32DebugConsole();
	Mars::CreateWin32Surface(hInstance, nShowCmd);

	game->Run();

	delete game;
	return 0;
}
#else
s32 main()
{
	system_info.core_count = std::thread::hardware_concurrency();

    auto game = Mars::CreateApplication();
    
    game->Run();
    
    delete game;
    return 0;
}
#endif
