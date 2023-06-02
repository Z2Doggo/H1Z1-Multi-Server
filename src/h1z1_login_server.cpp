#if defined(BASE_INTERNAL)
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctype>
#include <iostream>
#include <string>
#include <time>

#else
static void platform_win_console_write(char *format, ...);
#define printf(s, ...) platform_win_console_write(s, __VA_ARGS__)
#endif // BASE_INTERNAL

#define BASE_USE_ARENA 1
#define BASE_USE_STRING 1
#include "base.hpp"
#define BASE_PLATFORM_USE_SOCKETS 1
#include "base_platform.hpp"
#include "game_server.hpp"

#define LOCAL_PORT 20042
#define MAX_FRAGMENTS 12000
#define MAX_PACKET_LENGTH 512
#define DATA_HEADER_LENGTH 4
#define MAX_SESSIONS_COUNT 4

#include "./packets/login_udp_11_kotk.cpp"
#include "./shared/connection.hpp"
#include "./shared/protocol/fragment_pool.cpp"
#include "./shared/protocol/stream.hpp"
#include "./shared/session.hpp"
#include "./utilities/crypt_rc4.cpp"
#include "./utilities/endian.cpp"
#include "./utilities/util.cpp"
