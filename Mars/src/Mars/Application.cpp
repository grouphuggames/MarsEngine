#include "Application.h"
#include "vec3.h"
#include "vec4.h"
#include "Debug.h"
#include "Utils.h"
#include "Renderer.h"


namespace Mars
{
    Application::Application()
    {}

    Application::~Application()
    {}

    void Application::Run()
    {
		GameStartup();
		InitDX12();
		InitDX12Scene();
		dx12_data.command_list->Close();

		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (game_state.running)
		{
			TimerInfo info = {};
			info.time_scale = MARS_TIME::MARS_MILLISECOND;		// for framerate timer, this must be set to milliseconds
			StartTimer(info);

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// game time stuff goes here
			UpdateDX12Renderer();
			DX12Draw();

			StopTimer(info);
			game_state.framerate = 1000.f / info.time_delta;
		}

		TerminateDX12();
    }
}
