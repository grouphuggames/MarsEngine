#pragma once
#include "Log.h"


#include "Surface.h"
#ifdef _DEBUG
#include "Debug.h"
#endif

extern Mars::Application* Mars::CreateApplication();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	auto game = Mars::CreateApplication();

#ifdef _DEBUG
	CreateWin32DebugConsole();
#endif
	CreateWin32Surface(hInstance, nShowCmd);

	MARS_CORE_INFO("Systems Setup OK!");

	game->Run();
	delete game;

	return 0;
}
