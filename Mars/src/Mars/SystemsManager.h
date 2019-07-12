#pragma once
#include "Core.h"
#ifdef _WIN32
#include "Audio.h"
#endif
#include "Renderer.h"


namespace Mars
{
#ifdef _WIN32
	MARS_API Audio audio_system;
#endif

	void InitSystems()
	{
#ifdef _WIN32
		InitGL();
		InitGLScene();
		audio_system.Init();
#endif
	}

	void TerminateSystems()
	{
#ifdef _WIN32
		wglDeleteContext(game_state.render_context);
#endif
	}
}
