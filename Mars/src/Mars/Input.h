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

	MARS_API struct
	{
		std::unordered_map<s32, InputFunc> InputFuncs = {};
	} input_wrapper;

	void SetInput(MARS_KEY input, InputFunc func)
	{
		if (input_wrapper.InputFuncs.find(input) == input_wrapper.InputFuncs.end())
			input_wrapper.InputFuncs.insert({ input, func });
		else
			input_wrapper.InputFuncs.at(input) = func;
	}

	void SetInput(MARS_MOUSE input, InputFunc func)
	{
		if (input_wrapper.InputFuncs.find(input) == input_wrapper.InputFuncs.end())
			input_wrapper.InputFuncs.insert({ input, func });
		else
			input_wrapper.InputFuncs.at(input) = func;
	}

	void GetKeyboardInput()
	{
		if (GetAsyncKeyState(MARS_A))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_A)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_B))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_B)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_C))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_C)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_D))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_D)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_E))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_E)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_F))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_F)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_G))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_G)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_H))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_H)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_I))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_I)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_J))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_J)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_K))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_K)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_L))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_L)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_M))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_M)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_N))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_N)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_O))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_O)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_P))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_P)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_Q))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_Q)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_R))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_R)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_S))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_S)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_T))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_T)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_U))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_U)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_V))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_V)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_W))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_W)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_X))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_X)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_Y))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_Y)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_Z))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_Z)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}
	}

	f32 last_x = 0.f;
	f32 last_y = 0.f;
	bool first_mouse = true;

	void GetMouseInput()
	{
		POINT mouse_cursor;
		GetCursorPos(&mouse_cursor);

		if (first_mouse)
		{
			last_x = mouse_cursor.x;
			last_y = mouse_cursor.y;
			first_mouse = false;
		}

		f32 x_offset = last_x - mouse_cursor.x;
		f32 y_offset = last_y - mouse_cursor.y;
		last_x = mouse_cursor.x;
		last_y = mouse_cursor.y;

		f32 sensitivity = 0.1f;
		x_offset *= sensitivity;
		y_offset *= sensitivity;

		camera_data.yaw += x_offset;
		camera_data.pitch += y_offset;

		if (camera_data.pitch > 89.f)
		{
			camera_data.pitch = 89.f;
		}
		if (camera_data.pitch < -89.f)
		{
			camera_data.pitch = -89.f;
		}

		vec3 front;
		front.data.m128_f32[0] = cosf(ToRadians(camera_data.yaw)) * cosf(ToRadians(camera_data.pitch));
		front.data.m128_f32[1] = sinf(ToRadians(camera_data.pitch));
		front.data.m128_f32[2] = sinf(ToRadians(camera_data.yaw)) * cosf(ToRadians(camera_data.pitch));
		camera_data.camera_front = vec3::Normalize(front);

		if (GetAsyncKeyState(MARS_MOUSE_BUTTON_LEFT))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_MOUSE_BUTTON_LEFT)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}

		if (GetAsyncKeyState(MARS_MOUSE_BUTTON_RIGHT))
		{
			try
			{
				input_wrapper.InputFuncs.at(MARS_MOUSE_BUTTON_RIGHT)();
			}
			catch (const std::exception&)
			{
				MARS_CORE_ERROR("THAT FUNCTION HAS NOT YET BEEN SET!");
			}
		}
	}


#ifdef _WIN32
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

			case WM_CLOSE:
				game_state.running = false;
				PostQuitMessage(0);
				break;
			}
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
#endif
}
