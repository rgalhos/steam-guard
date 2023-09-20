#include "steam_guard.h"

int gen_auth_code(char *out, const char *shared_secret, const int server_time_diff)
{
    const char code_dict[27] = "23456789BCDFGHJKMNPQRTVWXY";
    const int code_dict_size = strlen(code_dict);

    unsigned char dec_shared_secret[32];
    int dec_shared_secret_len = EVP_DecodeBlock(dec_shared_secret, (unsigned char *)shared_secret, strlen(shared_secret));

    if (-1 == dec_shared_secret_len)
        return 1;

    int time = get_steam_time(server_time_diff) / 30;
    uint8_t time_array[8];
    char code_array[6];

    for (int i = 8; i > 0; i--)
    {
        time_array[i - 1] = time & 255;
        time >>= 8;
    }

    uint8_t *hdata = HMAC(EVP_sha1(), dec_shared_secret, dec_shared_secret_len, time_array, 8, NULL, NULL);
    uint8_t b = hdata[19] & 0xF;
    int codepoint = ((hdata[b] & 0x7F) << 24) |
                    ((hdata[b + 1] & 0xFF) << 16) |
                    ((hdata[b + 2] & 0xFF) << 8) |
                    (hdata[b + 3] & 0xFF);

    for (int i = 0; i < 5; i++)
    {
        code_array[i] = code_dict[codepoint % code_dict_size];
        codepoint /= code_dict_size;
    }

    code_array[5] = '\0';
    memcpy(out, code_array, 6);

    return 0;
} // gen_auth_code
