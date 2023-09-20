#include "server_time.h"

static size_t write_cb(void *data, size_t size, size_t nmemb, void *clientp)
{
    size_t total_size = size * nmemb;
    char *buffer = (char *)clientp;

    strncat(buffer, (char *)data, total_size);

    return total_size;
} // write_cb

int get_server_time_diff()
{
    CURL *curl;
    if (!(curl = curl_easy_init()))
        return 0;

    char res_buf[1024] = "";
    CURLcode res_code;

    curl_easy_setopt(curl, CURLOPT_URL, TWO_FACTOR_TIME_QUERY);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "steamid=0");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, STEAM_USERAGENT);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, res_buf);

    res_code = curl_easy_perform(curl);
    
    if (CURLE_OK != res_code)
        fprintf(stderr, "could not get steam's server time: %s\n", curl_easy_strerror(res_code));

    curl_easy_cleanup(curl);

    time_t server_time = 0;

    cJSON *json;
    if ((json = cJSON_ParseWithLength(res_buf, strlen(res_buf))) == NULL)
        return 0;

    do
    {
        cJSON *j_response = cJSON_GetObjectItemCaseSensitive(json, "response");
        if (!cJSON_IsObject(j_response))
            break;

        cJSON *j_server_time = cJSON_GetObjectItemCaseSensitive(j_response, "server_time");
        if (!cJSON_IsString(j_server_time) || (NULL == j_server_time->valuestring))
            server_time = atoi(j_server_time->valuestring);

    } while (0);

    cJSON_free(json);

    if (0 == server_time)
        return 0;

    return server_time - time(NULL);
} // get_server_time_diff

int get_steam_time(int server_time_diff)
{
    return time(NULL) + server_time_diff;
} // get_steam_time
