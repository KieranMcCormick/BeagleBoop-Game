//
// Created by jamie on 15/10/17.
//

#include "Joystick.h"
#include "Potentiometer.h"
#include "Accelerometer.h"
#include "InputManager.h"
#include <time.h>
#include <stdlib.h>

#define ACCELEROMETER_THRESHOLD 750
#define POTENTIOMETER_THRESHOLD 250
#define MILLISECONDS_PER_SECOND 1000

static int timeoutInMilliseconds;

static Input getJoystickInput(Direction joystickInput);
static Input getAccelerometerInput(Acceleration initialOrientation, Acceleration currentOrientation);
static char* inputStrings[NUMBER_OF_INPUTS] = {
        "Push UP on the joystick",
        "Push DOWN on the joystick",
        "Push LEFT on the joystick",
        "Push RIGHT on the joystick",
        "Push the CENTER on the joystick",
        "Tilt the board LEFT",
        "Tilt the board RIGHT",
        "Tilt the board FORWARD",
        "Tilt the board BACK",
        "Turn the potentiometer"
};

void InputManager_init(int specifiedTimeoutInMilliseconds){
    timeoutInMilliseconds = specifiedTimeoutInMilliseconds;
    Joystick_init();
    Potentiometer_init();
    Accelerometer_init();
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
    clock_t startTime = clock();
    Acceleration initialOrientation = Accelerometer_getAcceleration();
    int initialVoltage = Potentiometer_getValue();
    int millisecondsSinceStart = 0;
    while(millisecondsSinceStart < timeoutInMilliseconds){
        Direction joystickInput = Joystick_getDirection();
        if(joystickInput != NEUTRAL){
            *inputTimeMilliseconds = millisecondsSinceStart;
            return getJoystickInput(joystickInput);
        }
        Acceleration currentOrientation = Accelerometer_getAcceleration();
        if(abs(initialOrientation.x - currentOrientation.x) > ACCELEROMETER_THRESHOLD ||
                abs(initialOrientation.y - currentOrientation.y) > ACCELEROMETER_THRESHOLD){
            *inputTimeMilliseconds = millisecondsSinceStart;
            return getAccelerometerInput(initialOrientation, currentOrientation);
        }
        int currentVoltage = Potentiometer_getValue();
        if(abs(initialVoltage - currentVoltage) > POTENTIOMETER_THRESHOLD){
            *inputTimeMilliseconds = millisecondsSinceStart;
            return POTENTIOMETER_TURN;
        }
        millisecondsSinceStart = (double)(clock() - startTime) / CLOCKS_PER_SEC * MILLISECONDS_PER_SECOND;
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
        return ACCELEROMETER_TILT_BACK;
    }
    else if(currentOrientation.y - initialOrientation.y < (-ACCELEROMETER_THRESHOLD)){
        return ACCELEROMETER_TILT_FORWARD;
    }
    return NO_INPUT;
}