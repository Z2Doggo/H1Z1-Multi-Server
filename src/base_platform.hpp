//====================================================================================================
// Platform - Requires only core and platform-provided headers
//====================================================================================================

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#if defined(BASE_PLATFORM_USE_SOCKETS)
typedef struct Platform_Socket {
  b32 is_valid;
  uptr socket;
} Platform_Socket;
#endif // BASE_PLATFORM_USE_SOCKETS

#define PLATFORM_FOLDER_CREATE(name) b32 name(char *folder_path)
typedef PLATFORM_FOLDER_CREATE(platform_folder_create);

#define PLATFORM_BUFFER_WRITE_TO_FILE(name)                                    \
  u32 name(char *file_path, void *buffer, u32 size)
typedef PLATFORM_BUFFER_WRITE_TO_FILE(platform_buffer_write_to_file);

#define PLATFORM_BUFFER_LOAD_FROM_FILE(name)                                   \
  u32 name(char *file_path, void *buffer, u32 size)
typedef PLATFORM_BUFFER_LOAD_FROM_FILE(platform_buffer_load_from_file);

#define PLATFORM_WALL_CLOCK(name) u64 name(void)
typedef PLATFORM_WALL_CLOCK(platform_wall_clock);

#define PLATFORM_ELAPSED_SECONDS(name) f32 name(u64 begin, u64 end)
typedef PLATFORM_ELAPSED_SECONDS(platform_elapsed_seconds);

#if defined(BASE_PLATFORM_USE_SOCKETS)
#define PLATFORM_SOCKET_UDP_CREATE_AND_BIND(name) Platform_Socket name(u16 port)
typedef PLATFORM_SOCKET_UDP_CREATE_AND_BIND(
    platform_socket_udp_create_and_bind);

#define PLATFORM_RECEIVE_FROM(name)                                            \
  i32 name(Platform_Socket sock, void *buffer, i32 size, u32 *from_ip,         \
           u16 *from_port)
typedef PLATFORM_RECEIVE_FROM(platform_receive_from);

#define PLATFORM_SEND_TO(name)                                                 \
  i32 name(Platform_Socket sock, void *buffer, i32 size, u32 to_ip, u16 to_port)
typedef PLATFORM_SEND_TO(platform_send_to);
#endif // BASE_PLATFORM_USE_SOCKETS

struct Platform_Api {
  platform_folder_create *folder_create;
  platform_buffer_write_to_file *buffer_write_to_file;
  platform_buffer_load_from_file *buffer_load_from_file;
  platform_wall_clock *wall_clock;
  platform_elapsed_seconds *elapsed_seconds;
#if defined(BASE_PLATFORM_USE_SOCKETS)
  platform_socket_udp_create_and_bind *socket_udp_create_and_bind;
  platform_receive_from *receive_from;
  platform_send_to *send_to;
#endif // BASE_PLATFORM_USE_SOCKETS
};

#if defined(BASE_PLATFORM_WINDOWS)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <timeapi.h>

#include <limits.h>
// HACK:
global u64 global_performance_frequency;

#if defined(BASE_PLATFORM_USE_SOCKETS)
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

STATIC_ASSERT(SIZE_OF(SOCKET) == SIZE_OF(uptr));
#endif // BASE_PLATFORM_USE_SOCKETS

internal bool platform_win_folder_create(const char *folder_path) {
  if (!CreateDirectoryA(folder_path, NULL)) {
    DWORD last_error = GetLastError();
    if (last_error != ERROR_ALREADY_EXISTS) {
      std::cout << "[!] CreateDirectory error on \"" << folder_path
                << "\" - Error: " << last_error << std::endl;
      return false;
    }
  }

  return true;
}

internal PLATFORM_BUFFER_WRITE_TO_FILE(platform_win_buffer_write_to_file) {
  HANDLE file_handle = CreateFileA(file_path, FILE_APPEND_DATA, 0, NULL,
                                   OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (file_handle == INVALID_HANDLE_VALUE) {
    printf("[!] Unable to open file to write \"%s\" - Error: %u\n", file_path,
           GetLastError());
    return 0;
  }

  if (!buffer) {
    printf("[!] Buffer to write from is null\n");
    CloseHandle(file_handle);
    return 0;
  }

  DWORD bytes_written;
  if (!WriteFile(file_handle, buffer, size, &bytes_written, 0)) {
    printf("[!] Unable to write to file \"%s\" - Error: %u\n", file_path,
           GetLastError());
    CloseHandle(file_handle);
    return 0;
  }

  CloseHandle(file_handle);
  return bytes_written;
}

internal PLATFORM_BUFFER_LOAD_FROM_FILE(platform_win_buffer_load_from_file) {
  HANDLE file_handle = CreateFileA(file_path, GENERIC_READ, 0, 0, OPEN_EXISTING,
                                   FILE_ATTRIBUTE_NORMAL, NULL);

  if (file_handle == INVALID_HANDLE_VALUE) {
    printf("[!] Unable to open file to read \"%s\" - Error: %u\n", file_path,
           GetLastError());
    return 0;
  }

  if (!buffer) {
    printf("[!] Buffer to load to is null\n");
    CloseHandle(file_handle);
    return 0;
  }

  LARGE_INTEGER file_size;
  if (!GetFileSizeEx(file_handle, &file_size)) {
    printf("[!] Unable to get size of \"%s\" - Error: %u\n", file_path,
           GetLastError());
    CloseHandle(file_handle);
    return 0;
  }

  if (file_size.QuadPart > UINT_MAX) {
    printf("[!] File \"%s\" is too large to load\n", file_path);
    CloseHandle(file_handle);
    return 0;
  }

  if (file_size.LowPart > size) {
    printf("[!] Buffer is too small to load from file \"%s\"\n", file_path);
    CloseHandle(file_handle);
    return 0;
  }

  DWORD bytes_read;
  if (!ReadFile(file_handle, buffer, file_size.LowPart, &bytes_read, NULL)) {
    printf("[!] Unable to load from file \"%s\" - Error: %u\n", file_path,
           GetLastError());
    CloseHandle(file_handle);
    return 0;
  }

  CloseHandle(file_handle);
  return bytes_read;
}

internal PLATFORM_WALL_CLOCK(platform_win_wall_clock) {
  LARGE_INTEGER result;
  QueryPerformanceCounter(&result);
  return result.QuadPart;
}

internal PLATFORM_ELAPSED_SECONDS(platform_win_elapsed_seconds) {
  f32 result = (f32)(end - begin) / (f32)global_performance_frequency;
  return result;
}

#if defined(BASE_PLATFORM_USE_SOCKETS)
internal b32 platform_win_wsa_initialize() {
  WSADATA wsa_data;
  int error = WSAStartup(MAKEWORD(2, 2), &wsa_data);
  if (error) {
    printf("[!] WSAStartup failed - Error: %d\n", error);
    return FALSE;
  }

  return TRUE;
}

internal
PLATFORM_SOCKET_UDP_CREATE_AND_BIND(platform_win_socket_udp_create_and_bind) {
  local_persist b32 is_wsa_initialized;
  if (!is_wsa_initialized) {
    is_wsa_initialized = platform_win_wsa_initialize();
    ASSERT(is_wsa_initialized);
  }

  Platform_Socket result = {0};
  result.socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (result.socket == INVALID_SOCKET) {
    printf("[!] socket() failed: %d\n", WSAGetLastError());
    return result;
  }

  u_long cmd_arg = TRUE;
  if (ioctlsocket(result.socket, FIONBIO, &cmd_arg) == SOCKET_ERROR) {
    printf("[!] ioctlsocket() failed: %d\n", WSAGetLastError());
    goto socket_close;
  }

  SOCKADDR_IN addr = {addr.sin_family = AF_INET, addr.sin_port = htons(port),
                      addr.sin_addr.s_addr = htonl(INADDR_ANY)};

  int bind_result = bind(result.socket, (SOCKADDR *)&addr, sizeof(addr));
  if (bind_result == SOCKET_ERROR) {
    printf("[!] bind() failed: %d\n", WSAGetLastError());
    goto socket_close;
  }

  result.is_valid = TRUE;
  return result;

socket_close:
  if (closesocket(result.socket) == SOCKET_ERROR) {
    printf("[!] closesocket() failed: %d\n", WSAGetLastError());
  }
  return result;
}

internal int platform_win_receive_from(SOCKET socket, char *buffer, int size,
                                       unsigned int *from_ip,
                                       unsigned short *from_port) {
  sockaddr_in from_address = {0};
  int from_address_size = sizeof(from_address);

  int result =
      recvfrom(socket, buffer, size, 0,
               reinterpret_cast<sockaddr *>(&from_address), &from_address_size);

  if (result == SOCKET_ERROR) {
    int last_error = WSAGetLastError();
    if (last_error != WSAEWOULDBLOCK) {
      std::cout << "[!] recvfrom() failed: " << last_error << std::endl;
    }
    return 0;
  } else if (result == 0) {
    std::cout << "[!] recvfrom() returned 0" << std::endl;
    return 0;
  }

  *from_ip = ntohl(from_address.sin_addr.s_addr);
  *from_port = ntohs(from_address.sin_port);
  return result;
}

int platform_win_send_to(SOCKET socket, const char *buffer, int size,
                         unsigned int to_ip, unsigned short to_port) {
  sockaddr_in to_address = {0};
  to_address.sin_family = AF_INET;
  to_address.sin_addr.s_addr = htonl(to_ip);
  to_address.sin_port = htons(to_port);

  int result =
      sendto(socket, buffer, size, 0, reinterpret_cast<sockaddr *>(&to_address),
             sizeof(to_address));

  if (result == SOCKET_ERROR) {
    std::cout << "[!] sendto() failed: " << WSAGetLastError() << std::endl;
    return 0;
  }

  return result;
}

#endif // BASE_PLATFORM_USE_SOCKETS

#endif // BASE_PLATFORM_WINDOWS

// #endif // BASE_PLATFORM
