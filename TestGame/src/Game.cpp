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
	MARS_CORE_INFO(camera_data.camera_position);
}

void MoveBackward()
{
	f32 camera_speed = 2.5f * game_state.current_frame_time;
	camera_data.camera_position -= camera_speed * camera_data.camera_front;
	MARS_CORE_INFO(camera_data.camera_position);
}

void MoveLeft()
{
	f32 camera_speed = 2.5f * game_state.current_frame_time;
	camera_data.camera_position += Mars::vec3::Normalize(Mars::vec3::Cross(camera_data.camera_front, camera_data.camera_up)) * camera_speed;
	MARS_CORE_INFO(camera_data.camera_position);
}

void MoveRight()
{
	f32 camera_speed = 2.5f * game_state.current_frame_time;
	camera_data.camera_position -= Mars::vec3::Normalize(Mars::vec3::Cross(camera_data.camera_front, camera_data.camera_up)) * camera_speed;
	MARS_CORE_INFO(camera_data.camera_position);
}

// end of external functions

Mars::Application* Mars::CreateApplication()
{
	SetInput(MARS_W, MoveForward);
	SetInput(MARS_S, MoveBackward);
	SetInput(MARS_A, MoveLeft);
	SetInput(MARS_D, MoveRight);
	game_state.width = 1280;
	game_state.height = 720;

	return new Game();
}
