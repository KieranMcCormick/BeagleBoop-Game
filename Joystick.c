/*
 * Joystick.c
 *
 *  Created on: Oct 4, 2017
 *      Author: jamie
 */

#include "Joystick.h"
#include <stdio.h>
#include <stdlib.h>

#define LENGTH 8
#define EXPORT "/sys/class/gpio/export"
#define JOYSTICK_UP_PIN "26"
#define JOYSTICK_DOWN_PIN "46"
#define JOYSTICK_LEFT_PIN "65"
#define JOYSTICK_RIGHT_PIN "47"
#define JOYSTICK_CENTER_PIN "27"
#define JOYSTICK_UP_VALUE "/sys/class/gpio/gpio26/value"
#define JOYSTICK_DOWN_VALUE "/sys/class/gpio/gpio46/value"
#define JOYSTICK_LEFT_VALUE "/sys/class/gpio/gpio65/value"
#define JOYSTICK_RIGHT_VALUE "/sys/class/gpio/gpio47/value"
#define JOYSTICK_CENTER_VALUE "/sys/class/gpio/gpio27/value"

static void exportPins(void);
static void readPinValue(const char* fileName, char* result);
static void setPinValue(const char* fileName, const char* value);
static FILE* openFile(const char* fileName, const char* permissions);

void Joystick_init(void){
	exportPins();
}

void Joystick_cleanup(void){

}

Direction Joystick_getDirection(void){
	char value[(LENGTH)];
	readPinValue((JOYSTICK_UP_VALUE), value);
	if(value[0] == '0'){
		return UP;
	}
	readPinValue((JOYSTICK_DOWN_VALUE), value);
	if(value[0] == '0'){
		return DOWN;
	}
	readPinValue((JOYSTICK_LEFT_VALUE), value);
	if(value[0] == '0'){
		return LEFT;
	}
	readPinValue((JOYSTICK_RIGHT_VALUE), value);
	if(value[0] == '0'){
		return RIGHT;
	}
	readPinValue((JOYSTICK_CENTER_VALUE), value);
	if(value[0] == '0'){
		return CENTER;
	}
	else{
		return NEUTRAL;
	}
}

void exportPins(void){
	setPinValue((EXPORT), (JOYSTICK_UP_PIN));
	setPinValue((EXPORT), (JOYSTICK_DOWN_PIN));
	setPinValue((EXPORT), (JOYSTICK_LEFT_PIN));
	setPinValue((EXPORT), (JOYSTICK_RIGHT_PIN));
	setPinValue((EXPORT), (JOYSTICK_CENTER_PIN));
	return;
}

//openFile, readPinValue, setPinValue code from my assignment 1
static void readPinValue(const char* fileName, char* result){
	FILE* pinValue = openFile(fileName, "r");
	fgets(result, (LENGTH), pinValue);
	fclose(pinValue);
	return;
}

static void setPinValue(const char* fileName, const char* value){
	FILE* pinValue = openFile(fileName, "w");
	fprintf(pinValue, "%s", value);
	fclose(pinValue);
	return;
}

static FILE* openFile(const char* fileName, const char* permissions){
	FILE* filePointer = fopen(fileName, permissions);
	if (filePointer == NULL){
		printf("ERROR: Unable to open export or pin file.\n");
		exit(-1);
	}
	return filePointer;
}
