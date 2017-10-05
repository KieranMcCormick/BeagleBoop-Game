typedef struct
{
	int x;
	int y;
	int z;
} ACCELERATION;

void initAccelerometer();
void stopAccelerometer();
ACCELERATION getAcceleration();
