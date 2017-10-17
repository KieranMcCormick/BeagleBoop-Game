#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_core.h"
#include "seg_display.h"
#include "client_interface.h"
#include "InputManager.h"

static int gameOver;
static char *gameOverMessage;

static GAMESPEC gameSpec;
static STATS playerStats;

static char *playerName;

//TODO:
//Call buzzer

static int blacklistContains(int input)
{
	for(int i = 0;i < gameSpec.inputBlacklistSize;i++)
	{
		if(gameSpec.inputBlacklist[i] == input)
		{
			return 1;
		}
	}
	return 0;
}

static int getNextInput()
{
	int validInputs[NUMBER_OF_INPUTS];

	int j = 0;
	for(int i = 0;i < NUMBER_OF_INPUTS;i++)
	{
		if(!blacklistContains(i))
		{
			validInputs[j] = i;
			j++;
		}
	}
	
	return validInputs[rand() % (NUMBER_OF_INPUTS - gameSpec.inputBlacklistSize)];
}

void startGame()
{
	gameOver = 0;

	printf("Enter your name: ");
	scanf("%s", playerName);
	printf("Starting game!");

	gameSpec = getNewGameSpec(playerName);
	srand(gameSpec.sequenceSeed);
	InputManager_init(gameSpec.inputTime);

	playerStats.missCount = 0;
	playerStats.wrongInputCount = 0;
	playerStats.averageInputTime = gameSpec.inputTime;
	playerStats.score = 0;

	while(!gameOver)
	{
		int requestedInput = getNextInput();
		printf("%s!\n", InputManager_getInputString(requestedInput));

		int timeTaken;
		int actualInput = InputManager_waitForInput(&timeTaken);
		int totalIterations = playerStats.score + playerStats.wrongInputCount + playerStats.missCount + 1;
		playerStats.averageInputTime = (playerStats.averageInputTime * (totalIterations - 1) / totalIterations) + (timeTaken / totalIterations);

		if(actualInput != requestedInput)
		{
			if(actualInput == NO_INPUT)
			{
				playerStats.missCount++;
			}
			else
			{
				playerStats.wrongInputCount++;
			}

			int livesLeft = playerStats.wrongInputCount + playerStats.missCount - gameSpec.lives;
			if(livesLeft <= 0)
			{
				endGame("No lives left!");
			}
			else if(livesLeft == 1)
			{
				printf("Woops! 1 life left!\n");
			}
			else
			{
				printf("Woops! %d lives left!\n", livesLeft);
			}
		}
		else
		{
			playerStats.score++;
			displayNumber(playerStats.score);
		}
		reportPlayerStats(playerStats);
	}

	printf("Game Over!\n%s\n", gameOverMessage);
	free(gameOverMessage);
}

void endGame(char *message)
{
	int messageSize = strlen(message) + 1;
	gameOverMessage = malloc(messageSize * sizeof(char));
	strcpy(gameOverMessage, message);
	gameOver = 1;
}
