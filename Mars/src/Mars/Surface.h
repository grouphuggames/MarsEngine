#pragma once
#include "Log.h"


enum MARS_KEY
{
	MARS_A = 0x41,
	MARS_B = 0x42,
	MARS_C = 0x43,
	MARS_D = 0x44,
	MARS_E = 0x45,
	MARS_F = 0x46,
	MARS_G = 0x47,
	MARS_H = 0x48,
	MARS_I = 0x49,
	MARS_J = 0x4A,
	MARS_K = 0x4B,
	MARS_L = 0x4C,
	MARS_M = 0x4D,
	MARS_N = 0x4E,
	MARS_O = 0x4F,
	MARS_P = 0x50,
	MARS_Q = 0x51,
	MARS_R = 0x52,
	MARS_S = 0x53,
	MARS_T = 0x54,
	MARS_U = 0x55,
	MARS_V = 0x56,
	MARS_W = 0x57,
	MARS_X = 0x58,
	MARS_Y = 0x59,
	MARS_Z = 0x5A,
	MARS_QUIT = 0x1B
};

enum MARS_MOUSE
{
	MARS_MOUSE_BUTTON_LEFT = 0x01,
	MARS_MOUSE_BUTTON_RIGHT = 0x02
};

extern void TestFunc();

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case MARS_QUIT:
			PostQuitMessage(0);
			break;
		case MARS_A:
			MARS_CORE_INFO("A Key Pressed!");
			break;
		case MARS_B:
			MARS_CORE_INFO("B Key Pressed!");
			break;
		case MARS_C:
			MARS_CORE_INFO("C Key Pressed!");
			break;
		case MARS_D:
			MARS_CORE_INFO("D Key Pressed!");
			break;
		case MARS_E:
			MARS_CORE_INFO("E Key Pressed!");
			break;
		case MARS_F:
			MARS_CORE_INFO("F Key Pressed!");
			break;
		case MARS_G:
			MARS_CORE_INFO("G Key Pressed!");
			break;
		case MARS_H:
			MARS_CORE_INFO("H Key Pressed!");
			break;
		case MARS_I:
			MARS_CORE_INFO("I Key Pressed!");
			break;
		case MARS_J:
			MARS_CORE_INFO("J Key Pressed!");
			break;
		case MARS_K:
			MARS_CORE_INFO("K Key Pressed!");
			break;
		case MARS_L:
			MARS_CORE_INFO("L Key Pressed!");
			break;
		case MARS_M:
			MARS_CORE_INFO("M Key Pressed!");
			break;
		case MARS_N:
			MARS_CORE_INFO("N Key Pressed!");
			break;
		case MARS_O:
			MARS_CORE_INFO("O Key Pressed!");
			break;
		case MARS_P:
			MARS_CORE_INFO("P Key Pressed!");
			break;
		case MARS_Q:
			MARS_CORE_INFO("Q Key Pressed!");
			break;
		case MARS_R:
			MARS_CORE_INFO("R Key Pressed!");
			break;
		case MARS_S:
			MARS_CORE_INFO("S Key Pressed!");
			break;
		case MARS_T:
			MARS_CORE_INFO("T Key Pressed!");
			break;
		case MARS_U:
			MARS_CORE_INFO("U Key Pressed!");
			break;
		case MARS_V:
			MARS_CORE_INFO("V Key Pressed!");
			break;
		case MARS_W:
			MARS_CORE_INFO("W Key Pressed!");
			break;
		case MARS_X:
			MARS_CORE_INFO("X Key Pressed!");
			break;
		case MARS_Y:
			MARS_CORE_INFO("Y Key Pressed!");
			break;
		case MARS_Z:
			MARS_CORE_INFO("Z Key Pressed!");
			break;
		default:
			MARS_CORE_INFO_EXTRA("KEY CODE NOT YET SET FOR INPUT!!");
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		TestFunc();
		break;

	case WM_RBUTTONDOWN:
		MARS_CORE_INFO("Right Mouse Button Pressed!");
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void CreateWin32Surface(HINSTANCE hInstance, int nShowCmd)
{
	// create win32 window
	HWND hwnd = NULL;
	LPCTSTR window_name = "MarsEngineWindow";
	LPCTSTR window_title = "GameWindow";
	int32_t width = 800;
	int32_t height = 600;
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
		MARS_CORE_ERROR("Error Creating WIN32 Window!!");

	if (fullscreen)
		SetWindowLong(hwnd, GWL_STYLE, 0);

	ShowCursor(false);
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);
}
