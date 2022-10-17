#include <SDL.h>
#include "Game.h"

Game* game = nullptr;

int main(int argc, char* argv[])
{
	const int FPS = 60;
	const int TPS = 20;
	const int frameDelay = 1000 / FPS;
	const int tickDelay = 1000 / TPS;

	const int W = 1920;
	const int H = 1080;
	const int res = 4;
	const double aspect = (double)W / (double)H;

	Uint32 frameStart;
	int frameTime;

	Uint32 tickStart;

	int tickTime;
	int pause = 0;

	game = new Game();
	//std::cout << " HEJ" << std::endl;
	game->init("Hello", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, res, aspect, false);
	//std::cout << " HEJ" << std::endl;
	int xpos = 0;
	Uint32 nextTick = 0;

	while (game->running())
	{

		frameStart = SDL_GetTicks();
		tickStart = SDL_GetTicks();

		game->handleEvents(&pause);
		if (!pause)
		{
			if (tickStart >= nextTick)
			{
				game->update();
				nextTick = tickStart + 100;
			}
		}
		game->render();


		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->clean();

	return 0;
}
