#include <Mars.h>
#include <Mars/MMath.h>
#include <Mars/mat4.h>
#include <Mars/MVector.h>
#include <Mars/Audio.h>
#include <Mars/Debug.h>


class Game : public Mars::Application
{
public:
	Game() = default;

	// use this function to set up game data for each game to start with
	void GameStartup()
	{
		MARS_INFO("Game Is Running!\n");
	}
};

// external functions...they go to the engine and are a good way to get info from the game back into the engine
void Fire();

void Fire()
{
#ifdef _WIN32
	Mars::audio_system.PlayAudio("..\\..\\Mars\\res\\music.wav");
#endif
}

Mars::Application* Mars::CreateApplication()
{
	SetInput(MARS_MOUSE_BUTTON_LEFT, Fire);
	SetInput(MARS_MOUSE_BUTTON_RIGHT, HotReload);
	game_state.width = 800;
	game_state.height = 600;

	return new Game();
}
