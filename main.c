//#include <json/json.h>
#include <stdio.h>
#include <curl/curl.h>
#include <json/json.h>
int main()
{
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));

		curl_easy_cleanup(curl);
	}

        struct json_object *jobj;
        char *str = "{ \"msg-type\": [ \"0xdeadbeef\", \"irc log\" ], \
                \"msg-from\": { \"class\": \"soldier\", \"name\": \"Wixilav\" }, \
                \"msg-to\": { \"class\": \"supreme-commander\", \"name\": \"[Redacted]\" }, \
                \"msg-log\": [ \
                        \"soldier: Boss there is a slight problem with the piece offering to humans\", \
                        \"supreme-commander: Explain yourself soldier!\", \
                        \"soldier: Well they don't seem to move anymore...\", \
                        \"supreme-commander: Oh snap, I came here to see them twerk!\" \
                        ] \
                }";

        printf("str:\n---\n%s\n---\n\n", str);

        jobj = json_tokener_parse(str);
        printf("jobj from str:\n---\n%s\n---\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));

	return 0;
}
