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
		// this needs to be if guarded
		// WIN32 code
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (true)
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
