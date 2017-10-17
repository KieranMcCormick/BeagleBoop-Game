#include "game_core.h"
#include "seg_display.h"
#include "client_interface.h"

static int gameOver;
static char *gameOverMessage;

static GAMESPEC gameSpec;
static STATS playerStats;

static char *playerName;

//TODO:
//Call buzzer
//Keep track of score and set seg display

static int blacklistContains(int input)
{
	for(int i = 0;i < gameSpec.inputBlacklistSize;i++)
	{
		if(gameSpec.inputBlacklist[i] == input)
		{
			return 1;
		}
	}
	return false;
}

static void getNextInput()
{
	int validInputs[INPUT_COUNT];

	int j = 0;
	for(int i = 0;i < INPUT_COUNT;i++)
	{
		if(!blacklistContains(i))
		{
			validInputs[j] = i;
			j++;
		}
	}
	
	return validInputs[rand() % (INPUT_COUNT - gameSpec.inputBlacklistSize)];
}

void startGame()
{
	gameOver = 0;

	printf("Enter your name: ");
	scanf("%s", &playerName);
	printf("Starting game!");

	gameSpec = getNewGameSpec(playerName);
	srand(gameSpec.sequenceSeed);
	initInputMaster(gameSpec.inputTime);

	playerStats.missCount = 0;
	playerStats.wrongInputCount = 0;
	playerStats.averageInputTime = FLT_MAX;
	playerStats.score = 0;

	while(!gameOver)
	{
		int requestedInput = getNextInput();
		printf("%s!\n", inputActionStrings[requestedInput]);

		int success;
		clock_t startTime, endTime;

		startTime = clock();
		success = waitForInput(requestedInput);
		endTime = clock();

		int timeTaken = (endTime - startTime) * 1000 / CLOCKS_PER_SEC;
		int totalIterations = playerStats.score + playerStats.wrongInputCount + playerStats.missCount + 1;
		playerStats.averageInputTime = (playerStats.averageInputTime * (totalIterations - 1) / totalIterations) + (timeTaken / totalIterations);

		if(success != CORRECT_INPUT)
		{
			if(success == WRONG_INPUT)
			{
				playerStats.wrongInputCount++;
			}
			else if(success == NO_INPUT)
			{
				playerStats.missCount++;
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
			score++;
			displayNumber(score);
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
