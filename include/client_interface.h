typedef struct
{
	struct timespec_t inputTime;
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
} STATS;

GAMESPEC getNewGameSpec(char *playerName);
void reportPlayerStats(STATS playerStats);
