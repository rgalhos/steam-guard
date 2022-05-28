#include <cstdint>
#include <string>
#include <cstring>
#include <stdexcept>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include "ServerTime.cpp"

class SteamGuard
{
private:
    ServerTime serverTime;
    const uint8_t *sharedSecret = NULL;
    int sharedSecretLen = -1;
    const char codeDict[27] = "23456789BCDFGHJKMNPQRTVWXY";
    const int codeDictSize = strlen(codeDict);
    bool shouldAlign = false;

    void _init(const uint8_t *sharedSecret, int sharedSecretLen, bool shouldAlign)
    {
        this->sharedSecret = sharedSecret;
        this->sharedSecretLen = sharedSecretLen;
        this->shouldAlign = shouldAlign;
    }

public:
    SteamGuard(const uint8_t *sharedSecret, int sharedSecretLen, bool shouldAlign)
    {
        _init(sharedSecret, sharedSecretLen, shouldAlign);
    }

    SteamGuard(const uint8_t *sharedSecret, int sharedSecretLen)
    {
        _init(sharedSecret, sharedSecretLen, true);
    }

    std::string generateSteamGuardCode()
    {
        if (sharedSecret == NULL)
        {
            throw std::runtime_error("no shared secret");
        }
        else if (shouldAlign && !serverTime.isAligned())
        {
            serverTime.alignTime();
        }

        int time = serverTime.getSteamTime() / 30;
        uint8_t timeArray[8];

        for (int i = 8; i > 0; i--)
        {
            timeArray[i - 1] = time & 255;
            time >>= 8;
        }

        char codeArray[6];
        uint8_t *hashedData = HMAC(EVP_sha1(), sharedSecret, sharedSecretLen, timeArray, 8, NULL, NULL);
        uint8_t b = hashedData[19] & 0xF;
        int codepoint = ((hashedData[b] & 0x7F) << 24) |
                        ((hashedData[b + 1] & 0xFF) << 16) |
                        ((hashedData[b + 2] & 0xFF) << 8) |
                        (hashedData[b + 3] & 0xFF);

        for (int i = 0; i < 5; i++)
        {
            codeArray[i] = codeDict[codepoint % codeDictSize];
            codepoint /= codeDictSize;
        }

        codeArray[5] = '\0';

        return std::string(codeArray);
    }
};
