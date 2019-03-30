#include <Mars.h>
#include <Mars/MMath.h>
#include <Mars/MVector.h>
#include <Mars/Audio.h>
#include <string>


class Game : public Mars::Application
{
public:
	Game() 
	{
		Mars::MVector<s32> v;
		v.PushBack(1);
		v.PushBack(2);
		v.PushBack(3);

		Mars::MVector<s32> q = v;
		q.Remove(1);

		for (auto i : v)
		{
			OutputDebugStringA(std::to_string(i).c_str());
		}

		for (auto j : q)
		{
			OutputDebugStringA(std::to_string(j).c_str());
		}
	}
	~Game() {}
};

// external functions...they go to the engine and are a good way to get info from the game back into the engine
void Fire();
void Fire2();

void Fire()
{
	Mars::Audio audio;
	audio.Init();
	audio.PlayAudio("..\\..\\Mars\\res\\music.wav");
	Mars::SetInput(Mars::MARS_MOUSE_BUTTON_LEFT, Fire2);
}

void Fire2()
{
	Mars::SetInput(Mars::MARS_MOUSE_BUTTON_LEFT, Fire);
}

Mars::Application* Mars::CreateApplication()
{
	SetInput(MARS_MOUSE_BUTTON_LEFT, Fire);
	SetInput(MARS_MOUSE_BUTTON_RIGHT, HotReload);
	return new Game();
}
