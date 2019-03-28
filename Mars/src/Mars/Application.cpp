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
		MARS_CORE_INFO("Game Is Running!");

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

			StopTimer(info);
			game_state.framerate = 1000.f / info.time_delta;
			// MARS_CORE_INFO(game_state.framerate);
		}
    }
}
