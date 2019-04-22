#pragma once
#include "Core.h"
#include "Audio.h"
#include "Renderer.h"


namespace Mars
{
	MARS_API Audio audio_system;

	void InitSystems()
	{
		InitDX12();
		InitDX12Scene();
		dx12_data.command_list->Close();

		audio_system.Init();
	}

	void TerminateSystems()
	{
		TerminateDX12();
	}
}
