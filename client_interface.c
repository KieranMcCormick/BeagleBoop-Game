#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <curl/curl.h>
#include <json/json.h>

#include "client_interface.h"
#include "InputManager.h"
#include "game_core.h"

static bool gameRunning = true;

struct MemoryStruct
{
	char *memory;
	size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	mem->memory = realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL)
	{
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

void runNetGame()
{
	while (gameRunning)
	{

		CURL *curl;
		CURLcode res;

		struct MemoryStruct chunk;

		chunk.memory = malloc(1); /* will be grown as needed by the realloc above */
		chunk.size = 0;			  /* no data at this point */

		curl_global_init(CURL_GLOBAL_DEFAULT);

		curl = curl_easy_init();
		if (curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, "https://beagle-boop.firebaseio.com/boards/eddie.json");

			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

			curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

#ifdef SKIP_PEER_VERIFICATION

			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION

			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

			res = curl_easy_perform(curl);
			if (res != CURLE_OK)
			{
				fprintf(stderr, "curl_easy_perform() failed: %s\n",
						curl_easy_strerror(res));
			}
			else
			{
				//printf("%lu bytes retrieved\n", (long)chunk.size);
				//printf("recieved: %s \n", chunk.memory);
			}

			curl_easy_cleanup(curl);
		}
		char *str = chunk.memory;
		free(chunk.memory);

		curl_global_cleanup();

		struct json_object *jobj;

		jobj = json_tokener_parse(str);

		json_object *obj_alert = json_object_object_get(jobj, "alert");
		GAMESPEC newGameSpec;

		if (json_object_get_boolean(obj_alert))
		{
			json_object *inputTime = json_object_object_get(jobj, "timeToAnswer");
			newGameSpec.inputTime = (json_object_get_int(inputTime) * 1000);

			json_object *obj_lives = json_object_object_get(jobj, "lives");
			newGameSpec.lives = json_object_get_int(obj_lives);

			json_object *seed = json_object_object_get(jobj, "seed");
			newGameSpec.sequenceSeed = json_object_get_int(seed);

			json_object *blacklist = json_object_object_get(jobj, "blacklist");

			newGameSpec.inputBlacklistSize = json_object_array_length(blacklist);

			newGameSpec.inputBlacklist = malloc(sizeof(int) * newGameSpec.inputBlacklistSize);
			for (int i = 0; i < newGameSpec.inputBlacklistSize; i++)
			{
				json_object *arrayObj = json_object_array_get_idx(blacklist, i);
				const char *arrayObjVal = json_object_get_string(arrayObj);

				if (strcmp(arrayObjVal, "JOYSTICK_UP") == 0)
				{
					newGameSpec.inputBlacklist[i] = JOYSTICK_UP;
				}
				else if (strcmp(arrayObjVal, "JOYSTICK_DOWN") == 0)
				{
					newGameSpec.inputBlacklist[i] = JOYSTICK_DOWN;
				}
				else if (strcmp(arrayObjVal, "JOYSTICK_LEFT") == 0)
				{
					newGameSpec.inputBlacklist[i] = JOYSTICK_LEFT;
				}
				else if (strcmp(arrayObjVal, "JOYSTICK_RIGHT") == 0)
				{
					newGameSpec.inputBlacklist[i] = JOYSTICK_RIGHT;
				}
				else if (strcmp(arrayObjVal, "JOYSTICK_CENTER") == 0)
				{
					newGameSpec.inputBlacklist[i] = JOYSTICK_CENTER;
				}
				else if (strcmp(arrayObjVal, "ACCELEROMETER_TILT_LEFT") == 0)
				{
					newGameSpec.inputBlacklist[i] = ACCELEROMETER_TILT_LEFT;
				}
				else if (strcmp(arrayObjVal, "ACCELEROMETER_TILT_RIGHT") == 0)
				{
					newGameSpec.inputBlacklist[i] = ACCELEROMETER_TILT_RIGHT;
				}
				else if (strcmp(arrayObjVal, "ACCELEROMETER_PITCH_UP") == 0)
				{
					newGameSpec.inputBlacklist[i] = ACCELEROMETER_PITCH_UP;
				}
				else if (strcmp(arrayObjVal, "ACCELEROMETER_PITCH_DOWN") == 0)
				{
					newGameSpec.inputBlacklist[i] = ACCELEROMETER_PITCH_DOWN;
				}
				else if (strcmp(arrayObjVal, "POTENTIOMETER_TURN") == 0)
				{
					newGameSpec.inputBlacklist[i] = POTENTIOMETER_TURN;
				}
				else if (strcmp(arrayObjVal, "BUTTON_SEQUENCE") == 0)
				{
					newGameSpec.inputBlacklist[i] = BUTTON_SEQUENCE;
				}
			}
			startGame(newGameSpec);
		}
	}
}

static void curlPutResults(const char *message)
{
	CURL *curl;
	CURLcode res;
	struct curl_slist *headers = NULL;

	curl = curl_easy_init();

	if (curl)
	{
		headers = curl_slist_append(headers, "Content-Type: application/json");

		curl_easy_setopt(curl, CURLOPT_URL, "https://beagle-boop.firebaseio.com/boards/eddie/results.json");
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message); /* data goes here */

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
		}
		curl_easy_cleanup(curl);
	}
}

void reportPlayerStats(STATS playerStats)
{
	printf("Misses: %d, Incorrect: %d, Average Input Time: %f ms, score: %d\n", playerStats.missCount, playerStats.wrongInputCount, playerStats.averageInputTime, playerStats.score);

	json_object *jobj = json_object_new_object();

	json_object *missCount = json_object_new_int(playerStats.missCount);
	json_object_object_add(jobj, "missCount", missCount);

	json_object *wrongInputCount = json_object_new_int(playerStats.wrongInputCount);
	json_object_object_add(jobj, "wrongInputCount", wrongInputCount);

	json_object *averageInputTime = json_object_new_double(playerStats.averageInputTime);
	json_object_object_add(jobj, "averageInputTime", averageInputTime);

	json_object *score = json_object_new_int(playerStats.score);
	json_object_object_add(jobj, "score", score);

	curlPutResults(json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PLAIN));
}
