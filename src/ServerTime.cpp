#include <ctime>
#include <stdexcept>
#include <curl/curl.h>

#define TWO_FACTOR_TIME_QUERY "https://api.steampowered.com/ITwoFactorService/QueryTime/v0001"
#define STEAM_USERAGENT "Mozilla/5.0 (Linux; Android 10; Mi A2 Lite) AppleWebKit/537.36 (KHTML, like Gecko) " \
                        "Chrome/91.0.4472.120 Mobile Safari/537.36"

size_t writeCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

long getServerTime()
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();

    if (curl)
    {
        std::string responseBuffer;

        curl_easy_setopt(curl, CURLOPT_URL, TWO_FACTOR_TIME_QUERY);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "steamid=0");
        curl_easy_setopt(curl, CURLOPT_USERAGENT, STEAM_USERAGENT);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "could not get steam's server time: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);

        // TO DO: Use regular expressions or a JSON decoder
        std::string steamTime = responseBuffer.substr(28, 10);
        char *pEnd;

        return strtol(steamTime.c_str(), &pEnd, 10);
    }

    return 0;
}

class ServerTime
{
private:
    int timeDifference;
    bool aligned;

    std::time_t now()
    {
        return std::time(nullptr);
    }

    void _alignTime()
    {
        long t = getServerTime();

        if (t <= 0)
            throw std::runtime_error("invalid response");

        timeDifference = t - now();
        aligned = true;
    }

public:
    ServerTime()
    {
        timeDifference = 0;
        aligned = false;
    }

    void alignTime()
    {
        _alignTime();
    }

    bool isAligned()
    {
        return aligned;
    }

    int getSteamTime()
    {
        return now() + timeDifference;
    }
};
