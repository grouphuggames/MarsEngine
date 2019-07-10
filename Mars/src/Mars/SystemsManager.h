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
		InitGL();
		InitGLScene();

#ifdef _WIN32
		audio_system.Init();
#endif
	}

	void TerminateSystems()
	{
		wglDeleteContext(game_state.render_context);
	}
}
