#include <stdio.h>
#include <time.h>

#include "client_interface.h"

GAMESPEC getNewGameSpec(char *playerName)
{
	GAMESPEC newGameSpec;
	newGameSpec.inputTime = 8000;
	newGameSpec.lives = 3;
	newGameSpec.sequenceSeed = time(NULL);
	newGameSpec.inputBlacklist = NULL;
	newGameSpec.inputBlacklistSize = 0;
	return newGameSpec;
}

void reportPlayerStats(STATS playerStats)
{
	printf("Misses: %d, Incorrect: %d, Average Input Time: %f, score: %d\n", playerStats.missCount, playerStats.wrongInputCount, playerStats.averageInputTime, playerStats.score);
}
