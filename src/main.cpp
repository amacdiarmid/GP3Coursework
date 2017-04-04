#include "Window.h"
#undef main
#include "GP3Game.h"



int main(int argc, char * arg[])
{
	//set the game loop to true
	GameRunning = true;

	//create windows and openGL context
	//window name will e SPaceDecent
	createWindow("Space Decent");

	//hide mouse cursor 
	SDL_ShowCursor(SDL_DISABLE);

	//create new game
	GameApp *curGame = new GP3Game();

	//while game is running run the game loop
	while (GameRunning)
	{
		if (!curGame->gameLoop())
		{
			GameRunning = false;
		}
	}

	//destroy the game and window
	curGame->destroyGame();
	destroyWindow();

	return 0;
}
vector <IkeyboardListener*> keyboardListeners;
bool GameRunning = true;