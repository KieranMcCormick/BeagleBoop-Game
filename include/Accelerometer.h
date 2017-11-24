#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

typedef struct
{
	int x;
	int y;
	int z;
} Acceleration;

void Accelerometer_init();
void Accelerometer_cleanup();
Acceleration Accelerometer_getAcceleration();

#endif /* ACCELEROMETER_H */