#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "accelerometer.h"

static int i2cFile;

static void writeToFile(char *fileName, char *content)
{
	FILE *file = fopen(fileName, "w");
	if(file == NULL)
	{
		printf("ERROR: Unable to write \"%s\" to the file \"%s\".", content, fileName);
		exit(1);
	}
	fprintf(file, "%s", content);
	fclose(file);
}

void initAccelerometer()
{
	writeToFile("/sys/devices/platform/bone_capemgr/slots", "BB-I2C1"); //Enable bus

	unsigned char writeBuffer[2];
	int result;

	i2cFile = open("/dev/i2c-1", O_RDWR);
	if(i2cFile < 0)
	{
		printf("Unable to open /dev/i2c-1 for read/write.");
		exit(1);
	}

	result = ioctl(i2cFile, I2C_SLAVE, 0x1C);
	if(result < 0)
	{
		printf("Cannot set I2C device to slave address.");
		exit(1);
	}

	writeBuffer[0] = 0x2A;
	writeBuffer[1] = 0x01;
	result = write(i2cFile, writeBuffer, sizeof(writeBuffer));
	if(result != sizeof(writeBuffer))
	{
		printf("Cannot activate accelerometer.");
		exit(1);
	}
}

void stopAccelerometer()
{
	unsigned char writeBuffer[2];
	int result;

	writeBuffer[0] = 0x2A;
	writeBuffer[1] = 0x00;
	result = write(i2cFile, writeBuffer, sizeof(writeBuffer));
	if(result != sizeof(writeBuffer))
	{
		printf("Cannot deactivate accelerometer.");
	}
	close(i2cFile);
}

static unsigned char readByte(unsigned char address)
{
	int result = write(i2cFile, &address, sizeof(address));
	if(result != sizeof(address))
	{
		printf("Cannot write the accelerometer address to read.");
		exit(1);
	}

	unsigned char value = 0;
	result = read(i2cFile, &value, sizeof(value));
	if(result != sizeof(value))
	{
		printf("Cannot read acceleration.");
		exit(1);
	}

	printf("Reading address %d: %d\n", (int)address, (int)value);

	return value;
}

static int getAxisAcceleration(unsigned char address1, unsigned char address2)
{
	unsigned char accelerationBytes[2];
	accelerationBytes[0] = readByte(address1);
	accelerationBytes[1] = readByte(address2);

	return (((int)accelerationBytes[0]) << 4) + (((int)accelerationBytes[1]) >> 4);
}

ACCELERATION getAcceleration()
{
	ACCELERATION acceleration;
	acceleration.x = getAxisAcceleration(0x01, 0x02);
	acceleration.y = getAxisAcceleration(0x03, 0x04);
	acceleration.z = getAxisAcceleration(0x05, 0x06);
	return acceleration;
}
