#include <time.h>
#include <stdio.h>

#include "accelerometer.h"

int main()
{
	struct timespec delay;
	delay.tv_sec = 1;
	delay.tv_nsec = 0;
	
	initAccelerometer();
	for(int i = 0;i < 200;i++)
	{
		ACCELERATION accel = getAcceleration();
		printf("%d, %d, %d\n", accel.x, accel.y, accel.z);
		nanosleep(&delay, NULL);
	}
	stopAccelerometer();

	return 0;
}
