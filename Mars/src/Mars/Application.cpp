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

    void Application::Run(HWND hwnd)
    {
		// even though this is already set, it needs to be set again for some odd reason...has to do with MARS_API call
		//game_state.hwnd = hwnd;

		InitDX11();
		GameStartup();
		InitScene();		// for DX11 mode

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
			UpdateRenderer();		// for DX11 mode
			Draw();		// for DX11 mode

			StopTimer(info);
			game_state.framerate = 1000.f / info.time_delta;
			ShowFPSCounter(false);
		}

		TerminateDX11();
    }
}
