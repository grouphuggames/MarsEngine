#include "Application.h"
#include "vec3.h"
#include "vec4.h"
#include "Debug.h"
#include "Utils.h"


namespace Mars
{
    Application::Application()
    {}

    Application::~Application()
    {}

    void Application::Run()
    {
		GameStartup();
		InitSystems();

#ifdef _WIN32
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));
#endif

		while (game_state.running)
		{
			TimerInfo info = {};
			info.time_scale = MARS_TIME::MARS_MILLISECOND;		// for framerate timer, this must be set to milliseconds
			StartTimer(info);
#ifdef _WIN32
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
#endif

			// game time stuff goes here
			
			// update game data
			// render scene
			
			StopTimer(info);
			game_state.framerate = 1000.f / info.time_delta;
		}

		TerminateSystems();
    }
}
