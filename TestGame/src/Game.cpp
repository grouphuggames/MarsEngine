#include <Mars.h>


class Game : public Mars::Application
{
public:
	Game()
	{}

	~Game()
	{}
};

// external functions...they go to the engine and are a good way to get info from the game back into the engine
Mars::Application* Mars::CreateApplication()
{
	return new Game();
}


// as it is currently implemented, in order to change the name of the function in the game, the same change must be recorded in the engine
// to avoid this, maybe set input functions in an array, set them to index values in the engine
// just make sure that the index is set properly in the game
// extensive documentation will be needed...
void TestFunc()
{
	MARS_INFO("This funcion worked!!");
}
