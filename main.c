#include "game_core.h"
#include "client_interface.h"
#include "textToSpeech.h"

int main()
{
	TextToSpeech_init();
	GAMESPEC gameSettings = getNewGameSpec("test");
	startGame(gameSettings);
	TextToSpeech_cleanup();
	return 0;
}
