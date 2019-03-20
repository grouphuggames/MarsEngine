#include "Application.h"
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

			for (s32 i = 0; i < 100000; i++)
			{
				vec4 v1;
				vec4 v2(3.f);
				vec4 v3 = v1 + v2;
			}

			auto frame_stop = std::chrono::high_resolution_clock::now();
			f32 delta = std::chrono::duration_cast<std::chrono::milliseconds>(frame_stop - frame_start).count();
			game_state.framerate = 1000.f / delta;
		}
    }
}
