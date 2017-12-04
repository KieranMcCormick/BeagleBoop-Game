//
// Created by jamie on 15/10/17.
//

#include "Joystick.h"
#include "Potentiometer.h"
#include "Accelerometer.h"
#include "InputManager.h"
#include "Buttons.h"
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#define ACCELEROMETER_THRESHOLD 750
#define POTENTIOMETER_THRESHOLD 250
#define MILLISECONDS_PER_SECOND 1000
#define MICROSECONDS_PER_MILLISECOND 1000
#define EXTRA_TIME_FOR_BUTTONS 2000

static int timeoutInMilliseconds;
static int buttonTimeoutInMilliseconds;

static Input getJoystickInput(Direction joystickInput);
static Input getAccelerometerInput(Acceleration initialOrientation, Acceleration currentOrientation);
static char* inputStrings[NUMBER_OF_INPUTS];

void InputManager_init(int specifiedTimeoutInMilliseconds)
{
    timeoutInMilliseconds = specifiedTimeoutInMilliseconds;
    buttonTimeoutInMilliseconds = timeoutInMilliseconds + EXTRA_TIME_FOR_BUTTONS;	
    Joystick_init();
    Potentiometer_init();
    Accelerometer_init();

    inputStrings[JOYSTICK_UP] = "Push UP on the joystick";
    inputStrings[JOYSTICK_DOWN] = "Push DOWN on the joystick";
    inputStrings[JOYSTICK_LEFT] = "Push LEFT on the joystick";
    inputStrings[JOYSTICK_RIGHT] = "Push RIGHT on the joystick";
    inputStrings[JOYSTICK_CENTER] = "Push the CENTER on the joystick";
    inputStrings[ACCELEROMETER_TILT_LEFT] = "Tilt the board LEFT";
    inputStrings[ACCELEROMETER_TILT_RIGHT] = "Tilt the board RIGHT";
    inputStrings[ACCELEROMETER_PITCH_UP] = "Pitch the board UP";
    inputStrings[ACCELEROMETER_PITCH_DOWN] = "Pitch the board DOWN";
    inputStrings[POTENTIOMETER_TURN] = "Turn the potentiometer";
    inputStrings[BUTTON_SEQUENCE] = "Push the buttons in the order the lights flash";
}

void InputManager_cleanup(void){
    Joystick_cleanup();
    Potentiometer_cleanup();
    Accelerometer_cleanup();
    return;
}

char* InputManager_getInputString(Input input){
    return inputStrings[input];
}

Input InputManager_waitForInput(int* inputTimeMilliseconds){
    struct timeval startTime;
    struct timeval currentTime;
    gettimeofday(&startTime, NULL);
    Acceleration initialOrientation = Accelerometer_getAcceleration();
    int initialVoltage = Potentiometer_getValue();
    int millisecondsSinceStart = 0;
    *inputTimeMilliseconds = 0;
    while(millisecondsSinceStart < timeoutInMilliseconds){
        Direction joystickInput = Joystick_getDirection();
        if(joystickInput != NEUTRAL){
            return getJoystickInput(joystickInput);
        }
        Acceleration currentOrientation = Accelerometer_getAcceleration();
        if(abs(initialOrientation.x - currentOrientation.x) > (ACCELEROMETER_THRESHOLD) ||
                abs(initialOrientation.y - currentOrientation.y) > (ACCELEROMETER_THRESHOLD)){
            return getAccelerometerInput(initialOrientation, currentOrientation);
        }
        int currentVoltage = Potentiometer_getValue();
        if(abs(initialVoltage - currentVoltage) > (POTENTIOMETER_THRESHOLD)){
            return POTENTIOMETER_TURN;
        }
        gettimeofday(&currentTime, NULL);
        millisecondsSinceStart = (currentTime.tv_sec - startTime.tv_sec) * (MILLISECONDS_PER_SECOND);
        millisecondsSinceStart += (currentTime.tv_usec - startTime.tv_usec) / (MICROSECONDS_PER_MILLISECOND);
        *inputTimeMilliseconds = millisecondsSinceStart;
    }
    *inputTimeMilliseconds = timeoutInMilliseconds;
    return NO_INPUT;
}

static Input getJoystickInput(Direction joystickInput){
    switch(joystickInput){
        case UP:
            return JOYSTICK_UP;
        case DOWN:
            return JOYSTICK_DOWN;
        case LEFT:
            return JOYSTICK_LEFT;
        case RIGHT:
            return JOYSTICK_RIGHT;
        case CENTER:
            return JOYSTICK_CENTER;
        default:
            return NO_INPUT;
    }
}

static Input getAccelerometerInput(Acceleration initialOrientation, Acceleration currentOrientation){
    if(currentOrientation.x - initialOrientation.x > (ACCELEROMETER_THRESHOLD)){
        return ACCELEROMETER_TILT_RIGHT;
    }
    else if(currentOrientation.x - initialOrientation.x < (-ACCELEROMETER_THRESHOLD)){
        return ACCELEROMETER_TILT_LEFT;
    }
    else if(currentOrientation.y - initialOrientation.y > (ACCELEROMETER_THRESHOLD)){
        return ACCELEROMETER_PITCH_DOWN;
    }
    else if(currentOrientation.y - initialOrientation.y < (-ACCELEROMETER_THRESHOLD)){
        return ACCELEROMETER_PITCH_UP;
    }
    return NO_INPUT;
}

int InputManager_readButtonSequence(int* inputTimeMilliseconds, LED *ledFlashes, int ledFlashCount)
{
	struct timeval startTime;
	struct timeval currentTime;
	gettimeofday(&startTime, NULL);

	int millisecondsSinceStart = 0;

	printf("LED flash count: %d\n", ledFlashCount);

	for(int i = 0;i < ledFlashCount && millisecondsSinceStart < buttonTimeoutInMilliseconds;i++)
	{
		printf("LED flash expected: %d\n", ledFlashes[i]);

		int button;

		do
		{
			button = Buttons_getButton();
			gettimeofday(&currentTime, NULL);
			millisecondsSinceStart = (currentTime.tv_sec - startTime.tv_sec) * (MILLISECONDS_PER_SECOND);
			millisecondsSinceStart += (currentTime.tv_usec - startTime.tv_usec) / (MICROSECONDS_PER_MILLISECOND);
			*inputTimeMilliseconds = millisecondsSinceStart;

			if(millisecondsSinceStart >= buttonTimeoutInMilliseconds)
			{
				return 0;
			}
		}
		while(button != -1);

		do
		{
			button = Buttons_getButton();
			gettimeofday(&currentTime, NULL);
			millisecondsSinceStart = (currentTime.tv_sec - startTime.tv_sec) * (MILLISECONDS_PER_SECOND);
			millisecondsSinceStart += (currentTime.tv_usec - startTime.tv_usec) / (MICROSECONDS_PER_MILLISECOND);
			*inputTimeMilliseconds = millisecondsSinceStart;

			if(millisecondsSinceStart >= buttonTimeoutInMilliseconds)
			{
				return 0;
			}
		}
		while(button == -1);

		if(button != ledFlashes[i])
		{
			return 0;
		}
	}

	return 1;
}
