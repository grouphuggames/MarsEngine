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

void MoveForward()
{
	f32 camera_speed = 3.5f * game_state.current_frame_time;
	camera_data.camera_position += camera_speed * camera_data.camera_front;
	MARS_CORE_INFO(camera_data.camera_position);
}

void MoveBackward()
{
	f32 camera_speed = 3.5f * game_state.current_frame_time;
	camera_data.camera_position -= camera_speed * camera_data.camera_front;
	MARS_CORE_INFO(camera_data.camera_position);
}

Mars::Application* Mars::CreateApplication()
{
	SetInput(MARS_MOUSE_BUTTON_LEFT, Fire);
	SetInput(MARS_MOUSE_BUTTON_RIGHT, HotReload);
	SetInput(MARS_W, MoveForward);
	SetInput(MARS_S, MoveBackward);
	game_state.width = 800;
	game_state.height = 600;

	return new Game();
}
