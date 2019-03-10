#pragma once
#include "Log.h"


#ifdef ME_PLATFORM_WINDOWS

extern Mars::Application* Mars::CreateApplication();

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	auto game = Mars::CreateApplication();

	// create win32 window
	HWND hwnd = NULL;
	LPCTSTR window_name = "MarsEngineWindow";
	LPCTSTR window_title = "GameWindow";
	int width = 800;
	int height = 600;
	bool fullscreen = false;

	if (fullscreen)
	{
		HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(hmon, &mi);

		width = mi.rcMonitor.right - mi.rcMonitor.left;
		height = mi.rcMonitor.bottom - mi.rcMonitor.top;
	}

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = window_name;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(NULL, window_name, window_title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);
	if (!hwnd)
	{
		MARS_CORE_ERROR("Error Creating WIN32 Window!!");
		return -1;
	}

	if (fullscreen)
		SetWindowLong(hwnd, GWL_STYLE, 0);

	ShowCursor(false);
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	game->Run();
	delete game;

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hwnd);
			return 0;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

#endif