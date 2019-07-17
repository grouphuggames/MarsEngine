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

void MoveForward()
{
	f32 camera_speed = 2.5f * game_state.current_frame_time;
	camera_data.camera_position += camera_speed * camera_data.camera_front;
}

void MoveBackward()
{
	f32 camera_speed = 2.5f * game_state.current_frame_time;
	camera_data.camera_position -= camera_speed * camera_data.camera_front;
}

void MoveLeft()
{
	f32 camera_speed = 2.5f * game_state.current_frame_time;
	camera_data.camera_position += Mars::vec3::Normalize(Mars::vec3::Cross(camera_data.camera_front, camera_data.camera_up)) * camera_speed;
}

void MoveRight()
{
	f32 camera_speed = 2.5f * game_state.current_frame_time;
	camera_data.camera_position -= Mars::vec3::Normalize(Mars::vec3::Cross(camera_data.camera_front, camera_data.camera_up)) * camera_speed;
}

// end of external functions

Mars::Application* Mars::CreateApplication()
{
	SetInput(MARS_W, MoveForward);
	SetInput(MARS_S, MoveBackward);
	SetInput(MARS_A, MoveLeft);
	SetInput(MARS_D, MoveRight);
	game_state.window_width = 1280;
	game_state.window_height = 720;

	return new Game();
}
