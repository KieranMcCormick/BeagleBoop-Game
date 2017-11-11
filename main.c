#include "game_core.h"
#include "client_interface.h"

int main()
{
	GAMESPEC gameSettings = getNewGameSpec("test");
	startGame(gameSettings);
	return 0;
}
