//
// Created by jamie on 15/10/17.
//

#ifndef INPUTMANAGER_INPUTMANAGER_H
#define INPUTMANAGER_INPUTMANAGER_H

typedef enum {JOYSTICK_UP, JOYSTICK_DOWN, JOYSTICK_LEFT, JOYSTICK_RIGHT, JOYSTICK_CENTER,
                ACCELEROMETER_TILT_LEFT, ACCELEROMETER_TILT_RIGHT,
                ACCELEROMETER_PITCH_UP, ACCELEROMETER_PITCH_DOWN,
                POTENTIOMETER_TURN, NUMBER_OF_INPUTS, NO_INPUT}Input;

void InputManager_init(int specifiedTimeoutInMilliseconds);

void InputManager_cleanup(void);

char* InputManager_getInputString(Input input);

Input InputManager_waitForInput(int* inputTimeMilliseconds);

#endif /* INPUTMANAGER_H */
