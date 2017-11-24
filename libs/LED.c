//
// Created by jamie on 22/11/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "LED.h"

#define EXPORT "/sys/class/gpio/export"
#define RED_LED_PIN ("66")
#define RED_LED_VALUE ("/sys/class/gpio/gpio66/value")
#define RED_LED_DIRECTION ("/sys/class/gpio/gpio66/direction")
#define GREEN_LED_PIN ("69")
#define GREEN_LED_VALUE ("/sys/class/gpio/gpio69/value")
#define GREEN_LED_DIRECTION ("/sys/class/gpio/gpio69/direction")

#define LED_ON ("0")
#define LED_OFF ("1")
#define SET_OUTPUT ("out")

#define LED_DELAY (250000000)
static struct timespec delay = {0l,LED_DELAY};

static void prepareLEDs();
static void setPinValue(const char* fileName, const char* value);
static FILE* openFile(const char* fileName, const char* permissions);

void LED_init(){
    prepareLEDs();
}

void LED_cleanup(){

}

void LED_flashLED(LED led){
    if(led == RED){
        setPinValue(RED_LED_VALUE, LED_ON);
        nanosleep(&delay, (struct timespec*) NULL);
        setPinValue(RED_LED_VALUE, LED_OFF);
        nanosleep(&delay, (struct timespec*) NULL);
    }
    if(led == GREEN){
        setPinValue(GREEN_LED_VALUE, LED_ON);
        nanosleep(&delay, (struct timespec*) NULL);
        setPinValue(GREEN_LED_VALUE, LED_OFF);
        nanosleep(&delay, (struct timespec*) NULL);
    }
}

static void prepareLEDs(){
    setPinValue(EXPORT, RED_LED_PIN);
    setPinValue(RED_LED_DIRECTION, SET_OUTPUT);
    setPinValue(RED_LED_VALUE, LED_OFF);

    setPinValue(EXPORT, GREEN_LED_PIN);
    setPinValue(GREEN_LED_DIRECTION, SET_OUTPUT);
    setPinValue(GREEN_LED_VALUE, LED_OFF);
}

static void setPinValue(const char* fileName, const char* value){
    FILE* pinValue = openFile(fileName, "w");
    fprintf(pinValue, "%s", value);
    fclose(pinValue);
}

static FILE* openFile(const char* fileName, const char* permissions){
    FILE* filePointer = fopen(fileName, permissions);
    if (filePointer == NULL){
        printf("ERROR: Unable to open file: %s.\n", fileName);
        exit(-1);
    }
    return filePointer;
}
