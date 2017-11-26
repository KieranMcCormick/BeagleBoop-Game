/*
 * Buttons.c
 *
 *  Created on: Nov 26, 2017
 *      Author: eddie, copied from jamie
 */

#include "Buttons.h"
#include <stdio.h>
#include <stdlib.h>

#define LENGTH 8
#define EXPORT "/sys/class/gpio/export"
#define BUTTON_LEFT_PIN "67"
#define BUTTON_RIGHT_PIN "68"
#define BUTTON_LEFT_VALUE "/sys/class/gpio/gpio67/value"
#define BUTTON_RIGHT_VALUE "/sys/class/gpio/gpio68/value"

static void exportPins(void);
static void readPinValue(const char* fileName, char* result);
static void setPinValue(const char* fileName, const char* value);
static FILE* openFile(const char* fileName, const char* permissions);

void Buttons_init(void){
	exportPins();
}

void Buttons_cleanup(void){

}

int Buttons_getButton(void){
	char value[(LENGTH)];
	readPinValue((BUTTON_LEFT_VALUE), value);
	if(value[0] == '0'){
		return 0;
	}
	readPinValue((BUTTON_RIGHT_VALUE), value);
	if(value[0] == '0'){
		return 1;
	}
	else{
		return -1;
	}
}

static void exportPins(void){
	setPinValue((EXPORT), (BUTTON_LEFT_PIN));
	setPinValue((EXPORT), (BUTTON_RIGHT_PIN));
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
