#pragma once
#include "Core.h"
#include <unordered_map>


namespace Mars
{
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

	using InputFunc = void(*)(void);
	extern std::unordered_map<s32, InputFunc> InputFuncs = {};

	void SetInput(MARS_KEY input, InputFunc func)
	{
		if (InputFuncs.find(input) == InputFuncs.end())
			InputFuncs.insert({ input, func });
		else
			InputFuncs.at(input) = func;
	}

	void SetInput(MARS_MOUSE input, InputFunc func)
	{
		if (InputFuncs.find(input) == InputFuncs.end())
			InputFuncs.insert({ input, func });
		else
			InputFuncs.at(input) = func;
	}

	LRESULT CALLBACK ProcessInput(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_KEYDOWN:
			switch (wParam)
			{
			case MARS_QUIT:
				game_state.running = false;
				PostQuitMessage(0);
				break;
			case MARS_A:
				try
				{
					InputFuncs.at(MARS_A)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_B:
				try
				{
					InputFuncs.at(MARS_B)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_C:
				try
				{
					InputFuncs.at(MARS_C)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_D:
				try
				{
					InputFuncs.at(MARS_D)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_E:
				try
				{
					InputFuncs.at(MARS_E)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_F:
				try
				{
					InputFuncs.at(MARS_F)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_G:
				try
				{
					InputFuncs.at(MARS_G)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_H:
				try
				{
					InputFuncs.at(MARS_H)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_I:
				try
				{
					InputFuncs.at(MARS_I)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_J:
				try
				{
					InputFuncs.at(MARS_J)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_K:
				try
				{
					InputFuncs.at(MARS_K)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_L:
				try
				{
					InputFuncs.at(MARS_L)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_M:
				try
				{
					InputFuncs.at(MARS_M)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_N:
				try
				{
					InputFuncs.at(MARS_N)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_O:
				try
				{
					InputFuncs.at(MARS_O)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_P:
				try
				{
					InputFuncs.at(MARS_P)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_Q:
				try
				{
					InputFuncs.at(MARS_Q)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_R:
				try
				{
					InputFuncs.at(MARS_R)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_S:
				try
				{
					InputFuncs.at(MARS_S)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_T:
				try
				{
					InputFuncs.at(MARS_T)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_U:
				try
				{
					InputFuncs.at(MARS_U)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_V:
				try
				{
					InputFuncs.at(MARS_V)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_W:
				try
				{
					InputFuncs.at(MARS_W)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_X:
				try
				{
					InputFuncs.at(MARS_X)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_Y:
				try
				{
					InputFuncs.at(MARS_Y)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			case MARS_Z:
				try
				{
					InputFuncs.at(MARS_Z)();
				}
				catch (const std::exception&)
				{
					MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
				}
				break;
			default:
				MARS_CORE_ERROR("KEY CODE NOT YET SET FOR INPUT!!");
				break;
			}
			break;

		case WM_LBUTTONDOWN:
			try
			{
				InputFuncs.at(MARS_MOUSE_BUTTON_LEFT)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
			break;

		case WM_RBUTTONDOWN:
			try
			{
				InputFuncs.at(MARS_MOUSE_BUTTON_RIGHT)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
			break;

		case WM_CLOSE:
			game_state.running = false;
			PostQuitMessage(0);
			break;
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
