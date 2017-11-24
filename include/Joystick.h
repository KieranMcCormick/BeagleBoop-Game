/*
 * Joystick.h
 *
 *  Created on: Oct 4, 2017
 *      Author: jamie
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

typedef enum {NEUTRAL, UP, DOWN, LEFT, RIGHT, CENTER}Direction;

void Joystick_init(void);

void Joystick_cleanup(void);

Direction Joystick_getDirection(void);

#endif /* JOYSTICK_H_ */
