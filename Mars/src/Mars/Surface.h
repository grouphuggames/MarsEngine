#pragma once
#include "Core.h"
#include "Input.h"


namespace Mars
{
#ifdef _WIN32
	void CreateWin32Surface(HINSTANCE hInstance, s32 nShowCmd)
	{
		// create win32 window
		LPCTSTR window_name = "MarsEngineWindow";

		if (game_state.fullscreen)
		{
			HMONITOR hmon = MonitorFromWindow(game_state.hwnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFO mi = { sizeof(mi) };
			GetMonitorInfo(hmon, &mi);

			game_state.window_width = mi.rcMonitor.right - mi.rcMonitor.left;
			game_state.window_height = mi.rcMonitor.bottom - mi.rcMonitor.top;
		}

		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = ProcessInput;
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

		game_state.hwnd = CreateWindowEx(NULL, window_name, game_state.window_title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, game_state.window_width, game_state.window_height, NULL, NULL, hInstance, NULL);
		if (!game_state.hwnd)
		{
			MARS_CORE_ERROR("Error Creating WIN32 Window!!");
		}

		if (game_state.fullscreen)
		{
			SetWindowLong(game_state.hwnd, GWL_STYLE, 0);
		}

		ShowCursor(false);
		ShowWindow(game_state.hwnd, nShowCmd);
		UpdateWindow(game_state.hwnd);
	}
#endif
}
