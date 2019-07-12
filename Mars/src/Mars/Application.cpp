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

		TimerInfo engine_timer_info = {};
		engine_timer_info.time_scale = MARS_TIME::MARS_MILLISECOND;
		StartTimer(engine_timer_info);

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
			// update game data

			// render scene
#ifdef _WIN32
			RenderScene();
#endif

			StopTimer(info);
			game_state.framerate = 1000.f / info.time_delta;

			StopTimer(engine_timer_info);
			game_state.elapsed_time = engine_timer_info.time_delta / 1000.f;
		}

		TerminateSystems();
    }
}
