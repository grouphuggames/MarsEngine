#include <Mars.h>


class Game : public Mars::Application
{
public:
	Game()
	{}

	~Game()
	{}
};

Mars::Application* Mars::CreateApplication()
{
	return new Game();
}
