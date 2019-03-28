#include <Mars.h>
#include <Mars/MMath.h>


class Game : public Mars::Application
{
public:
	Game() {}
	~Game() {}
};

// external functions...they go to the engine and are a good way to get info from the game back into the engine
void Fire();
void Fire2();

void Fire()
{
	MARS_INFO(HALF_PI);
	Mars::SetInput(Mars::MARS_MOUSE_BUTTON_LEFT, Fire2);
}

void Fire2()
{
	MARS_INFO(PI);
	Mars::SetInput(Mars::MARS_MOUSE_BUTTON_LEFT, Fire);
}

Mars::Application* Mars::CreateApplication()
{
	SetInput(MARS_MOUSE_BUTTON_LEFT, Fire);
	SetInput(MARS_MOUSE_BUTTON_RIGHT, HotReload);
	return new Game();
}
