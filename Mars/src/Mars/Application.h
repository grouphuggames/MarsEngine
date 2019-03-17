#pragma once
#include "Core.h"


bool running = true;	// global variable to be cleaned up later

namespace Mars
{
    class MARS_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
    };

	Application* CreateApplication();
}
