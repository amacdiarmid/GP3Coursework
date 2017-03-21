#include "Window.h"
#undef main
#include "GP3Game.h"



int main(int argc, char * arg[])
{

	GameRunning = true;

	createWindow("Space Decent");

	GameApp *curGame = new GP3Game();

	while (GameRunning)
	{
		if (!curGame->gameLoop())
		{
			GameRunning = false;
		}
	}

	curGame->destroyGame();
	destroyWindow();

	return 0;
}
vector <IkeyboardListener*> keyboardListeners;
bool GameRunning = true;