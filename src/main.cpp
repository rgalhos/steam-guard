#include <iostream>
#include <cstring>
#include <openssl/evp.h>
#include <curl/curl.h>
#include "SteamGuard.cpp"

int main(int argc, char **argv)
{
    bool shouldAlignTime = false; // --align

    if (argc == 1)
    {
        std::cout << "Usage: " << argv[0] << " [sharedSecret]" << std::endl;
        std::cout << " --align - Get Steam's server time" << std::endl;
        return 1;
    }
    else
    {
        for (int i = 2; i < argc; i++)
        {
            char *arg = argv[i];

            if (!strcmp(arg, "--align"))
                shouldAlignTime = true;
        }
    }

    const unsigned char *sharedSecretRaw = (unsigned char *)argv[1];
    const int sharedSecretRawLen = strlen(argv[1]);

    unsigned char sharedSecret[32];
    int sharedSecretLen = EVP_DecodeBlock(sharedSecret, sharedSecretRaw, sharedSecretRawLen);

    if (sharedSecretLen == -1)
    {
        fprintf(stderr, "could not decode shared secret");
        return 1;
    }

    SteamGuard sg((uint8_t *)sharedSecret, sharedSecretLen, shouldAlignTime);
    std::string code = sg.generateSteamGuardCode();

    std::cout << code << std::endl;

    curl_global_cleanup();

    return 0;
}
