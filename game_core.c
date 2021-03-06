#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "game_core.h"
#include "seg_display.h"
#include "client_interface.h"
#include "InputManager.h"
#include "textToSpeech.h"
#include "LED.h"

static int gameOver;
static char *gameOverMessage;

static GAMESPEC gameSpec;
static STATS playerStats;

//static char *playerName;

static struct timespec delay = {2l, 0l};

static LED *ledFlashes;
static int ledFlashCount;

static int blacklistContains(int input)
{
	for (int i = 0; i < gameSpec.inputBlacklistSize; i++)
	{
		if (gameSpec.inputBlacklist[i] == input)
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
	for (int i = 0; i < NUMBER_OF_INPUTS; i++)
	{
		if (!blacklistContains(i))
		{
			validInputs[j] = i;
			j++;
		}
	}

	int nextInput = validInputs[rand() % (NUMBER_OF_INPUTS - gameSpec.inputBlacklistSize)];

	if (nextInput == BUTTON_SEQUENCE)
	{
		ledFlashCount = (rand() % 4) + 2;
		ledFlashes = malloc(sizeof(LED) * ledFlashCount);

		for (int i = 0; i < ledFlashCount; i++)
		{
			ledFlashes[i] = rand() % NUMBER_OF_LEDS;
		}
	}

	return nextInput;
}

void *flashLEDS()
{
	for (int i = 0; i < ledFlashCount; i++)
	{
		LED_flashLED(ledFlashes[i]);
	}

	return NULL;
}

void *startGame(GAMESPEC g)
{
	gameOver = 0;

	/* 	printf("Enter your name: ");
	scanf("%s", playerName);
	printf("Starting game!\n"); */

	gameSpec = g;
	srand(gameSpec.sequenceSeed);
	InputManager_init(gameSpec.inputTime);
	displayNumber(0);

	playerStats.missCount = 0;
	playerStats.wrongInputCount = 0;
	playerStats.averageInputTime = gameSpec.inputTime;
	playerStats.score = 0;

	while (!gameOver)
	{
		int requestedInput = getNextInput();
		printf("%s!\n", InputManager_getInputString(requestedInput));
		TextToSpeech_speak(requestedInput);

		int timeTaken;
		int actualInput;

		if (requestedInput == BUTTON_SEQUENCE)
		{
			pthread_t ledFlashingThreadID;
			pthread_create(&ledFlashingThreadID, NULL, &flashLEDS, NULL);

			//call InputManager or whatev to check button presses
			if (InputManager_readButtonSequence(&timeTaken, ledFlashes, ledFlashCount))
			{
				actualInput = BUTTON_SEQUENCE;
			}
			else
			{
				actualInput = -1;
			}

			pthread_join(ledFlashingThreadID, NULL);
			free(ledFlashes);
		}
		else
		{
			actualInput = InputManager_waitForInput(&timeTaken);
		}

		int totalIterations = playerStats.score + playerStats.wrongInputCount + playerStats.missCount + 1;
		playerStats.averageInputTime = (playerStats.averageInputTime * (totalIterations - 1) / totalIterations) + (timeTaken / totalIterations);

		if (actualInput != requestedInput)
		{
			if (actualInput == NO_INPUT)
			{
				playerStats.missCount++;
			}
			else
			{
				playerStats.wrongInputCount++;
			}

			int livesLeft = gameSpec.lives - (playerStats.wrongInputCount + playerStats.missCount);
			if (livesLeft <= 0)
			{
				endGame("No lives left!");
			}
			else if (livesLeft == 1)
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
		nanosleep(&delay, (struct timespec *)NULL);
	}

	reportPlayerStats(playerStats);

	printf("Game Over!\n%s\n", gameOverMessage);
	free(gameOverMessage);

	return NULL;
}

void endGame(char *message)
{
	int messageSize = strlen(message) + 1;
	gameOverMessage = malloc(messageSize * sizeof(char));
	strcpy(gameOverMessage, message);
	gameOver = 1;
}
