#include <Mars.h>
#include <Mars/vec4.h>


class Game : public Mars::Application
{
public:
	Game()
	{}

	~Game()
	{}
};

// external functions...they go to the engine and are a good way to get info from the game back into the engine
void Fire()
{
	MARS_INFO("maria eats guava");
}

void AimDownSight()
{
	MARS_INFO("Aiming Sights!");
}

void MoveForward()
{
	MARS_INFO("Moving Forward!");
}

Mars::Application* Mars::CreateApplication()
{
	SetInput(MARS_MOUSE_BUTTON_LEFT, Fire);
	SetInput(MARS_MOUSE_BUTTON_RIGHT, AimDownSight);
	SetInput(MARS_W, MoveForward);
	return new Game();
}
