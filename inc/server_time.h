#ifndef SERVER_TIME_H
#define SERVER_TIME_H

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cJSON.h"

#define TWO_FACTOR_TIME_QUERY "https://api.steampowered.com/ITwoFactorService/QueryTime/v0001"
#define STEAM_USERAGENT "Mozilla/5.0 (Linux; Android 10; Mi A2 Lite) AppleWebKit/537.36 (KHTML, like Gecko) " \
                        "Chrome/91.0.4472.120 Mobile Safari/537.36"

int get_steam_time(int);
int get_server_time_diff();

#endif // SERVER_TIME_H
