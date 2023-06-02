// NOTE(rhett): Fix complaints by the VS address sanitizer
#define STBSP__ASAN __declspec(no_sanitize_address)
#define STB_SPRINTF_IMPLEMENTATION

#include <Windows.h>
#include <cstdarg>
#include <cstdio>
#include <iostream>

// #if defined(BASE_INTERNAL)
////#include <stdio.h>
////#include <stdlib.h>
////#include <assert.h>
// #else
// static void platform_win_console_write(char* format, ...);
// #define printf(s, ...) platform_win_console_write(s, __VA_ARGS__)
// #endif // BASE_INTERNAL

#ifndef BASE_INTERNAL
void platform_win_console_write(const char *format, ...) {
  va_list args;
  va_start(args, format);

  HANDLE standard_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  char buffer[1024] = {0};
  int to_write = vsnprintf(buffer, sizeof(buffer), format, args);

  DWORD written;
  WriteConsole(standard_handle, buffer, to_write, &written, NULL);
  // OutputDebugString(buffer);

  va_end(args);
}
#endif // BASE_INTERNAL

#if defined(TERMINAL_UI)
#define printf
#endif // TERMINAL_UI

// #define TICK_RATE          60.0f

#if defined(TERMINAL_UI)
#define DISPLAY_RATE (TICK_RATE / 2.0f)
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 30
#define SCREEN_RESOLUTION (SCREEN_WIDTH * SCREEN_HEIGHT)
#endif // TERMINAL_UI

typedef b32 Key_States[0xff];

typedef struct App_State App_State;
typedef struct App_Memory App_Memory;
struct App_Memory {
  // Platform_State platform_state;
  Platform_Api platform_api;
  Buffer backing_memory;
  App_State *app_state;
#if defined(TERMINAL_UI)
  Buffer screen;
#endif // TERMINAL_UI
  f32 tick_ms;
  f32 work_ms;
  u64 tick_count;
  Key_States key_states;
};

#if defined(BASE_INTERNAL)
#define APP_TICK(name) void name(App_Memory *app_memory, b32 should_reload)
#else
#define APP_TICK(name) void name(App_Memory *app_memory)
#endif // BASE_INTERNAL
typedef APP_TICK(app_tick_t);