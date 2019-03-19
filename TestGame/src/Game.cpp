#include <Mars.h>


class Game : public Mars::Application
{
public:
	Game()
	{}

	~Game()
	{}
};

// as it is currently implemented, in order to change the name of the function in the game, the same change must be recorded in the engine
// to avoid this, maybe set input functions in an array, set them to index values in the engine
// just make sure that the index is set properly in the game
// extensive documentation will be needed...
void Fire()
{
	MARS_INFO("Fire!!");
}

void AimDownSight()
{
	MARS_INFO("Aiming Sights!");
}

void MoveForward()
{
	MARS_INFO("Moving Forward!");
}

// external functions...they go to the engine and are a good way to get info from the game back into the engine
Mars::Application* Mars::CreateApplication()
{
	SetInput(MARS_MOUSE_BUTTON_LEFT, Fire);
	SetInput(MARS_MOUSE_BUTTON_RIGHT, AimDownSight);
	SetInput(MARS_W, MoveForward);
	return new Game();
}
