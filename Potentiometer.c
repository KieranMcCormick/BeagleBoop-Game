/*
 * Potentiometer.c
 *
 *  Created on: Oct 4, 2017
 *      Author: jamie
 */

#include "Potentiometer.h"
#include <stdio.h>
#include <stdlib.h>

#define ENABLE_A2D_FILE "/sys/devices/platform/bone_capemgr/slots"
#define ENABLE_A2D_VALUE "BB-ADC"
#define POTENTIOMETER_VALUE "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"

static void enableA2D();
static FILE* openFile(const char* fileName, const char* permissions);

void Potentiometer_init(void){
	enableA2D();
	FILE* potFile = NULL;
	while(potFile == NULL){
		potFile = fopen((POTENTIOMETER_VALUE), "r");
	}
	fclose(potFile);
	return;
}

void Potentiometer_cleanup(void){
	return;
}

int Potentiometer_getValue(void){
	FILE* potFile= openFile((POTENTIOMETER_VALUE), "r");
	int potValue = 0;
	int readSuccess = fscanf(potFile, "%d", &potValue);
	if(readSuccess < 1){
		printf("ERROR: Unable to read potentiometer value.\n");
		exit(-1);
	}
	fclose(potFile);
	return potValue;
}

static void enableA2D(){
	FILE* A2DFile = openFile((ENABLE_A2D_FILE), "w");
	fprintf(A2DFile, "%s", (ENABLE_A2D_VALUE));
	fclose(A2DFile);
	return;
}

static FILE* openFile(const char* fileName, const char* permissions){
	FILE* filePointer = fopen(fileName, permissions);
	if (filePointer == NULL){
		printf("ERROR: Unable to open slots or pot file.\n");
		exit(-1);
	}
	return filePointer;
}
