#pragma once
#include "Core.h"
#include "SystemsManager.h"


namespace Mars
{
    class MARS_API Application
    {
    public:
        Application();
        virtual ~Application();

		virtual void GameStartup() = 0;
		void Run();
    };

	Application* CreateApplication();
}
