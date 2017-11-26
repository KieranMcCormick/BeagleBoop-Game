#include "game_core.h"
#include "client_interface.h"
#include "textToSpeech.h"
#include "seg_display.h"
#include "LED.h"
#include "Buttons.h"

int main()
{
	TextToSpeech_init();
	runSegDisplay();
	LED_init();
	Buttons_init();

	GAMESPEC gameSettings = getNewGameSpec("test");
	startGame(gameSettings);

	TextToSpeech_cleanup();
	endSegDisplay();
	LED_cleanup();
	Buttons_cleanup();

	return 0;
}
