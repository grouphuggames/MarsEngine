#include <Mars.h>
#include <Mars/vec3.h>


class Game : public Mars::Application
{
public:
	Game() {}
	~Game() {}
};

// external functions...they go to the engine and are a good way to get info from the game back into the engine
void Fire()
{
	MARS_INFO("maria eats guava");
}

Mars::Application* Mars::CreateApplication()
{
	SetInput(MARS_MOUSE_BUTTON_LEFT, Fire);
	SetInput(MARS_MOUSE_BUTTON_RIGHT, HotReload);
	ADDHOT(thisisavariable);
	return new Game();
}
