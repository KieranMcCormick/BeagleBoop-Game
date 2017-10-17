#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "seg_display.h"

static DIGIT leftDigit, rightDigit;

static pthread_mutex_t leftDigitLock;
static pthread_mutex_t rightDigitLock;

static int end = 0;

static unsigned char lowDigitValues[11];
static unsigned char highDigitValues[11];

static int i2cFile;

static pthread_t displayThreadID;

static void writeToFile(char *fileName, char *content)
{
	FILE *file = fopen(fileName, "w");
	if (file == NULL)
	{
		printf("ERROR: Unable to write \"%s\" to the file \"%s\".", content, fileName);
		exit(1);
	}
	fprintf(file, "%s", content);
	fclose(file);
}

static void writeDigit(DIGIT *digit, pthread_mutex_t *lock)
{
	unsigned char lowBits, highBits;
	unsigned char writeBuffer[2];
	int result;
	pthread_mutex_lock(lock);
	lowBits = (*digit).lowBits;
	highBits = (*digit).highBits;
	pthread_mutex_unlock(lock);
	writeBuffer[0] = 0x14;
	writeBuffer[1] = lowBits;
	result = write(i2cFile, writeBuffer, 2);
	if (result != 2)
	{
		printf("Cannot write low bits to left digit.");
		exit(1);
	}
	writeBuffer[0] = 0x15;
	writeBuffer[1] = highBits;
	result = write(i2cFile, writeBuffer, 2);
	if (result != 2)
	{
		printf("Cannot write high bits to left digit.");
		exit(1);
	}
}

static void *maintainDisplay()
{
	TIMESPEC *onTime = malloc(sizeof(TIMESPEC));
	(*onTime).tv_sec = 0;
	(*onTime).tv_nsec = 5000000; //5 milliseconds
	while (!end)
	{
		writeToFile("/sys/class/gpio/gpio44/value", "0"); //Turn off right digit
		writeDigit(&leftDigit, &leftDigitLock);	//write left digit
		writeToFile("/sys/class/gpio/gpio61/value", "1"); //Turn on left digit
		nanosleep(onTime, NULL); //Wait for inputs to register and lights to go on
		writeToFile("/sys/class/gpio/gpio61/value", "0"); //Turn off left digit
		writeDigit(&rightDigit, &rightDigitLock); //write right digit
		writeToFile("/sys/class/gpio/gpio44/value", "1"); //Turn on right digit
		nanosleep(onTime, NULL); //Wait for inputs to register and lights to go on
	}
	close(i2cFile);
	writeToFile("/sys/class/gpio/gpio44/value", "0"); //Turn off right digit
	free(onTime);
	return NULL;
}

void displayNumber(int number)
{
	int leftDigitValue, rightDigitValue;

	if(number > 99)
	{
		leftDigitValue = 10;
		rightDigitValue = 10;
	}
	else
	{
		leftDigitValue = number / 10; //Integer division
		rightDigitValue = number % 10;
	}

	pthread_mutex_lock(&leftDigitLock);
	leftDigit.lowBits = lowDigitValues[leftDigitValue];
	leftDigit.highBits = highDigitValues[leftDigitValue];
	pthread_mutex_unlock(&leftDigitLock);
	pthread_mutex_lock(&rightDigitLock);
	rightDigit.lowBits = lowDigitValues[rightDigitValue];
	rightDigit.highBits = highDigitValues[rightDigitValue];
	pthread_mutex_unlock(&rightDigitLock);
}

void runSegDisplay()
{
	pthread_mutex_init(&leftDigitLock, NULL);
	pthread_mutex_init(&rightDigitLock, NULL);

	lowDigitValues[0] = 161;  //1 + 32 + 128
	lowDigitValues[1] = 128;  //128
	lowDigitValues[2] = 49;   //1 + 16 + 32
	lowDigitValues[3] = 176;  //16 + 32 + 128
	lowDigitValues[4] = 144;  //16 + 128
	lowDigitValues[5] = 176;  //16 + 32 + 128
	lowDigitValues[6] = 177;  //1 + 16 + 32 + 128
	lowDigitValues[7] = 2;	//2
	lowDigitValues[8] = 177;  //1 + 16 + 32 + 128
	lowDigitValues[9] = 144;  //16 + 128
	lowDigitValues[10] = 10; //2 + 8

	highDigitValues[0] = 134; //2 + 4 + 128
	highDigitValues[1] = 2;   //2
	highDigitValues[2] = 14;  //2 + 4 + 8
	highDigitValues[3] = 14;  //2 + 4 + 8
	highDigitValues[4] = 138; //2 + 8 + 128
	highDigitValues[5] = 140; //4 + 8 + 128
	highDigitValues[6] = 140; //4 + 8 + 128
	highDigitValues[7] = 20;  //4 + 16
	highDigitValues[8] = 142; //2 + 4 + 8 + 128
	highDigitValues[9] = 142; //2 + 4 + 8 + 128
	highDigitValues[10] = 80; //16 + 64

	leftDigit.lowBits = 0;
	leftDigit.highBits = 0;
	rightDigit.lowBits = 0;
	rightDigit.highBits = 0;

	writeToFile("/sys/devices/platform/bone_capemgr/slots", "BB-I2C1"); //Enable bus
	writeToFile("/sys/class/gpio/export", "61"); //Enables left digit
	writeToFile("/sys/class/gpio/gpio61/direction", "out"); //Sets left digit to be output
	writeToFile("/sys/class/gpio/export", "44"); //Enables right digit
	writeToFile("/sys/class/gpio/gpio44/direction", "out"); //Sets right digit to be output

	unsigned char writeBuffer[2];
	int result;

	i2cFile = open("/dev/i2c-1", O_RDWR);
	if (i2cFile < 0)
	{
		printf("Unable to open /dev/i2c-1 for read/write.");
		exit(1);
	}

	result = ioctl(i2cFile, I2C_SLAVE, 0x20);
	if (result < 0)
	{
		printf("Cannot set I2C device to slave address.");
		exit(1);
	}

	writeBuffer[0] = 0x00;
	writeBuffer[1] = 0x00;
	result = write(i2cFile, writeBuffer, 2);
	if (result != 2)
	{
		printf("Cannot set direction of GPIO extender (0x00).");
		exit(1);
	}

	writeBuffer[0] = 0x01;
	writeBuffer[1] = 0x00;
	result = write(i2cFile, writeBuffer, 2);
	if (result != 2)
	{
		printf("Cannot set direction of GPIO extender (0x01).");
		exit(1);
	}

	pthread_create(&displayThreadID, NULL, &maintainDisplay, NULL);
}

void endSegDisplay()
{
	end = 1;
	pthread_join(displayThreadID, NULL);
}
