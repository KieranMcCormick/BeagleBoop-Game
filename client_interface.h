#ifndef CLIENT_INTERFACE_H
#define CLIENT_INTERFACE_H

typedef struct
{
	int inputTime; //milliseconds
	int lives;
	unsigned sequenceSeed;
	int *inputBlacklist;
	int inputBlacklistSize;
} GAMESPEC;

typedef struct
{
	int missCount;
	int wrongInputCount;
	float averageInputTime;
	int score;
} STATS;

GAMESPEC getNewGameSpec(char *playerName);
void reportPlayerStats(STATS playerStats);

#endif
