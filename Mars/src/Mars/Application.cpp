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
        while (true);
    }
}
