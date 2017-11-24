#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "Accelerometer.h"

static int i2cFile;

static void writeToFile(char *fileName, char *content)
{
	FILE *file = fopen(fileName, "w");
	if(file == NULL)
	{
		printf("ERROR: Unable to write \"%s\" to the file \"%s\".\n", content, fileName);
		exit(1);
	}
	fprintf(file, "%s", content);
	fclose(file);
}

void Accelerometer_init()
{
	writeToFile("/sys/devices/platform/bone_capemgr/slots", "BB-I2C1"); //Enable bus

	unsigned char writeBuffer[2];
	int result;

	i2cFile = open("/dev/i2c-1", O_RDWR);
	if(i2cFile < 0)
	{
		printf("Unable to open /dev/i2c-1 for read/write.\n");
		exit(1);
	}

	result = ioctl(i2cFile, I2C_SLAVE, 0x1C);
	if(result < 0)
	{
		printf("Cannot set I2C device to slave address.\n");
		exit(1);
	}

	writeBuffer[0] = 0x2A;
	writeBuffer[1] = 0x01;
	result = write(i2cFile, writeBuffer, sizeof(writeBuffer));
	if(result != sizeof(writeBuffer))
	{
		printf("Cannot activate accelerometer.\n");
		exit(1);
	}
}

void Accelerometer_cleanup()
{
	unsigned char writeBuffer[2];
	int result;

	writeBuffer[0] = 0x2A;
	writeBuffer[1] = 0x00;
	result = write(i2cFile, writeBuffer, sizeof(writeBuffer));
	if(result != sizeof(writeBuffer))
	{
		printf("Cannot deactivate accelerometer.\n");
	}
	close(i2cFile);
}

static int castBytesToInt(unsigned char byte1, unsigned char byte2)
{
	//Organizes the first byte into the least significant bits 5-12, and the 4 most
	//significant bits of the second byte into the 4 least significant bits
	int result = (((int)byte1) << 4) + (((int)byte2) >> 4);

	if(byte1 & 128) //Checks most significant bit (The negative indicator)
	{
		//-4096 is all 1s except for the last 12 bits. This is used to convert a 12 bit
		//negative 2's complement number into a n-bit 2's complement number
		return result | -4096;
	}
	else
	{
		return result;
	}
}

Acceleration Accelerometer_getAcceleration()
{
	unsigned char readAddress = 0x00;
	int result = write(i2cFile, &readAddress, 1);
	if(result != 1)
	{
		printf("Cannot write the accelerometer address to read. %d\n", result);
		exit(1);
	}

	unsigned char value[7];
	result = read(i2cFile, &value, sizeof(value));
	if(result != sizeof(value))
	{
		printf("Cannot read acceleration.\n");
		exit(1);
	}

	//printf("Reading address %d: %d\n", (int)address, (int)value);

	Acceleration acceleration;
	acceleration.x = castBytesToInt(value[1], value[2]);
	acceleration.y = castBytesToInt(value[3], value[4]);
	acceleration.z = castBytesToInt(value[5], value[6]);
	return acceleration;
}
