#ifndef STEAM_GUARD_H
#define STEAM_GUARD_H

#include <stdint.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include "server_time.h"

int gen_auth_code(char *, const char *, const int);

#endif // STEAM_GUARD_H
