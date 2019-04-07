#include <Mars.h>
#include <Mars/MMath.h>
#include <Mars/MVector.h>
#include <Mars/Audio.h>
#include <string>


class Game : public Mars::Application
{
public:
	Game() = default;

	// use this function to set up game data for each game to start with
	void GameStartup()
	{
		MARS_INFO("Game Is Running!");
	}
};

// external functions...they go to the engine and are a good way to get info from the game back into the engine
void Fire();

void Fire()
{
	Mars::Audio audio;
	audio.Init();
	audio.PlayAudio("..\\..\\Mars\\res\\music.wav");
}

Mars::Application* Mars::CreateApplication()
{
	SetInput(MARS_MOUSE_BUTTON_LEFT, Fire);
	return new Game();
}
