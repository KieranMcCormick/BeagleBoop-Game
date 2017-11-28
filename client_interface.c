#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "client_interface.h"
#include "InputManager.h"

GAMESPEC getNewDefaultGameSpec(char *playerName)
{
	GAMESPEC newGameSpec;
	newGameSpec.inputTime = 4000;
	newGameSpec.lives = 3;
	newGameSpec.sequenceSeed = time(NULL);
	newGameSpec.inputBlacklistSize = 1;
	newGameSpec.inputBlacklist = malloc(sizeof(int) * newGameSpec.inputBlacklistSize);
	newGameSpec.inputBlacklist[0] = BUTTON_SEQUENCE;
	return newGameSpec;
}

GAMESPEC getNewButtonsGameSpec(char *playerName)
{
	GAMESPEC newGameSpec;
	newGameSpec.inputTime = 8000;
	newGameSpec.lives = 3;
	newGameSpec.sequenceSeed = time(NULL);
	newGameSpec.inputBlacklistSize = NUMBER_OF_INPUTS - 1;
	newGameSpec.inputBlacklist = malloc(sizeof(int) * newGameSpec.inputBlacklistSize);

	int j = 0;
	for(int i = 0;i < NUMBER_OF_INPUTS;i++)
	{
		if(i != BUTTON_SEQUENCE)
		{
			newGameSpec.inputBlacklist[j] = i;
			j++;
		}
	}

	return newGameSpec;
}

void reportPlayerStats(STATS playerStats)
{
	printf("Misses: %d, Incorrect: %d, Average Input Time: %f ms, score: %d\n", playerStats.missCount, playerStats.wrongInputCount, playerStats.averageInputTime, playerStats.score);
}
