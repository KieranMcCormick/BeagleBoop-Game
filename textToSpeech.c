#include "audioMixer.h"
#include "InputManager.h"

char *soundFiles[NUMBER_OF_INPUTS];
wavedata_t audioFiles[NUMBER_OF_INPUTS];

void TextToSpeech_init()
{
	AudioMixer_init();

	soundFiles[JOYSTICK_UP] = "joystick/up";
	soundFiles[JOYSTICK_DOWN] = "joystick/down";
	soundFiles[JOYSTICK_LEFT] = "joystick/left";
	soundFiles[JOYSTICK_RIGHT] = "joystick/right";
	soundFiles[JOYSTICK_CENTER] = "joystick/in";
	soundFiles[ACCELEROMETER_TILT_LEFT] = "accelerometer/left";
	soundFiles[ACCELEROMETER_TILT_RIGHT] = "accelerometer/right";
	soundFiles[ACCELEROMETER_TILT_FORWARD] = "accelerometer/forward";
	soundFiles[ACCELEROMETER_TILT_BACK] = "accelerometer/back";
	soundFiles[POTENTIOMETER_TURN] = "potentiometer/turn";

	for(int i = 0;i < NUMBER_OF_INPUTS;i++)
	{
		AudioMixer_readWaveFileIntoMemory(soundFiles[i], &(audioFiles[i]));
	}
}

void TextToSpeech_cleanup()
{
	AudioMixer_cleanup();

	for(int i = 0;i < NUMBER_OF_INPUTS;i++)
	{
		AudioMixer_freeWaveFileData(&(audioFiles[i]));
	}
}

void TextToSpeech_speak(int sound)
{
	AudioMixer_queueSound(&(audioFiles[sound]));
}
