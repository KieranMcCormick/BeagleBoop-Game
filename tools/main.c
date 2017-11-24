#include "game_core.h"
#include "client_interface.h"
#include "textToSpeech.h"
#include "seg_display.h"

int main()
{
	TextToSpeech_init();
	runSegDisplay();

	GAMESPEC gameSettings = getNewGameSpec("test");
	startGame(gameSettings);

	TextToSpeech_cleanup();
	endSegDisplay();

	return 0;
}
