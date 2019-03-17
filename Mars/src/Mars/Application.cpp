#include "Application.h"
#include "Log.h"


namespace Mars
{
    Application::Application()
    {
		MARS_CORE_INFO("Game Created Successfully!");
	}

    Application::~Application()
    {}

    void Application::Run()
    {
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (running)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				// run game code
			}
		}
    }
}
