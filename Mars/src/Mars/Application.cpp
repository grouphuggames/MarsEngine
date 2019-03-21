#include "Application.h"
#include "vec3.h"
#include "vec4.h"
#include <chrono>


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

		vec3 s1(1.f);
		vec3 s2(1.f, 3.f, 3.f);
		vec3 s3;

		while (game_state.running)
		{
			auto frame_start = std::chrono::high_resolution_clock::now();

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// run game code

			vec3 l = vec3::Cross(s1, s2);
			f32 x = l.x();
			f32 y = l.y();
			f32 z = l.z();

			auto frame_stop = std::chrono::high_resolution_clock::now();
			f32 delta = std::chrono::duration_cast<std::chrono::nanoseconds>(frame_stop - frame_start).count();		// --this needs to be in milliseconds for framerate to be correct... if it does not say milliseconds, it is because of profiling/debugging efforts
			game_state.framerate = 1000.f / delta;
			MARS_CORE_INFO(game_state.framerate);
		}
    }
}
