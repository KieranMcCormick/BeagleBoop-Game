//
// Created by jamie on 22/11/17.
//

#ifndef LED_H
#define LED_H

typedef enum {RED, GREEN, NUMBER_OF_LEDS}LED;

void LED_init();

void LED_cleanup();

void LED_flashLED(LED led);

#endif //LED_H
