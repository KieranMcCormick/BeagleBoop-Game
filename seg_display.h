typedef struct timespec TIMESPEC;
typedef struct
{
	unsigned char lowBits;
	unsigned char highBits;
} DIGIT;

void displayNumber(int number);
void runSegDisplay();
void endSegDisplay();
