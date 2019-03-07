#pragma once


#ifdef ME_PLATFORM_WINDOWS

extern Mars::Application* Mars::CreateApplication();

int main(int argc, char** argv)
{
	auto game = Mars::CreateApplication();
	game->Run();
	delete game;
}

#endif