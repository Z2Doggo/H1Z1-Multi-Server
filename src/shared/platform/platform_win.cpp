#include <iostream>

struct Platform_State {
  LARGE_INTEGER frequency;
  b32 is_wsa_initialized;
};

struct Platform_Socket {
  b32 is_valid;
  SOCKET socket;
};

internal b32 platform_win_wsa_startup(Platform_State *state) {
  if (state->is_wsa_initialized) {
    std::cout << "[!] WSA already initialized" << std::endl;
    return TRUE;
  }

  WSADATA wsa_data;
  int error = WSAStartup(MAKEWORD(2, 2), &wsa_data);
  if (error) {
    std::cout << "[!] WSAStartup failed - Error: " << error << std::endl;
    return FALSE;
  }

  return state->is_wsa_initialized = TRUE;
}

internal b32 platform_win_folder_create(const char *folder_path) {
  if (!CreateDirectoryA(folder_path, NULL)) {
    DWORD last_error = GetLastError();
    if (last_error != ERROR_ALREADY_EXISTS) {
      std::cout << "[!] CreateDirectory error on \"" << folder_path
                << "\" - Error: " << last_error << std::endl;
      return FALSE;
    }
  }

  return TRUE;
}

internal UINT platform_win_buffer_write_to_file(const char *file_path,
                                                const void *buffer,
                                                UINT buffer_length) {
  HANDLE file_handle = CreateFileA(file_path, GENERIC_WRITE, 0, NULL,
                                   CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (file_handle == INVALID_HANDLE_VALUE) {
    std::cout << "[!] Unable to open file to write \"" << file_path
              << "\" - Error: " << GetLastError() << std::endl;
    return 0;
  }

  if (!buffer) {
    std::cout << "[!] Buffer to write from is null" << std::endl;
    CloseHandle(file_handle);
    return 0;
  }

  DWORD bytes_written;
  if (!WriteFile(file_handle, buffer, buffer_length, &bytes_written, NULL)) {
    std::cout << "[!] Unable to write to file \"" << file_path
              << "\" - Error: " << GetLastError() << std::endl;
    CloseHandle(file_handle);
    return 0;
  }

  CloseHandle(file_handle);
  return bytes_written;
}

internal UINT platform_win_buffer_load_from_file(const char *file_path,
                                                 void *buffer,
                                                 UINT buffer_length) {
  HANDLE file_handle = CreateFileA(file_path, GENERIC_READ, 0, 0, OPEN_EXISTING,
                                   FILE_ATTRIBUTE_NORMAL, NULL);

  if (file_handle == INVALID_HANDLE_VALUE) {
    std::cout << "[!] Unable to open file to read \"" << file_path
              << "\" - Error: " << GetLastError() << std::endl;
    return 0;
  }

  if (!buffer) {
    std::cout << "[!] Buffer to load to is null" << std::endl;
    CloseHandle(file_handle);
    return 0;
  }

  LARGE_INTEGER file_length;
  if (!GetFileSizeEx(file_handle, &file_length)) {
    std::cout << "[!] Unable to get size of \"" << file_path
              << "\" - Error: " << GetLastError() << std::endl;
    CloseHandle(file_handle);
    return 0;
  }

  if (file_length.QuadPart > UINT_MAX) {
    std::cout << "[!] File \"" << file_path << "\" is too large to load"
              << std::endl;
    CloseHandle(file_handle);
    return 0;
  }

  // TODO: make sure we want LowPart
  if (file_length.LowPart > buffer_length) {
    std::cout << "[!] Buffer is too small to load from file \"" << file_path
              << "\"" << std::endl;
    CloseHandle(file_handle);
    return 0;
  }

  DWORD bytes_read;
  if (!ReadFile(file_handle, buffer, file_length.LowPart, &bytes_read, NULL)) {
    std::cout << "[!] Unable to load from file \"" << file_path
              << "\" - Error: " << GetLastError() << std::endl;
    CloseHandle(file_handle);
    return 0;
  }

  CloseHandle(file_handle);
  return bytes_read;
}

internal Platform_Socket
platform_win_socket_udp_create_and_bind(Platform_State *state, UINT port) {
  if (!state->is_wsa_initialized) {
    platform_win_wsa_startup(state);
  }

  Platform_Socket result = {0};
  result.socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (result.socket == INVALID_SOCKET) {
    std::cout << "[!] socket() failed: " << WSAGetLastError() << std::endl;
    return result;
  }

  // Use non-blocking socket
  u_long cmd_arg = TRUE;
  if (ioctlsocket(result.socket, FIONBIO, &cmd_arg) == SOCKET_ERROR) {
    std::cout << "[!] ioctlsocket() failed: " << WSAGetLastError() << std::endl;
    goto socket_close;
  }

  SOCKADDR_IN addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  int bind_result = bind(result.socket, (SOCKADDR *)&addr, sizeof(addr));
  if (bind_result == SOCKET_ERROR) {
    std::cout << "[!] bind() failed: " << WSAGetLastError() << std::endl;
    goto socket_close;
  }

  result.is_valid = TRUE;
  return result;

socket_close:
  if (closesocket(result.socket) == SOCKET_ERROR) {
    std::cout << "[!] closesocket() failed: " << WSAGetLastError() << std::endl;
  }
  return result;
}

internal int platform_win_receive_from(Platform_Socket sock, void *buffer,
                                       int length, int *from_ip,
                                       int *from_port) {
  SOCKADDR_IN from_address = {0};
  int from_address_length = sizeof(from_address);

  int result = recvfrom(sock.socket, static_cast<char *>(buffer), length, 0,
                        (struct sockaddr *)&from_address, &from_address_length);

  if (result == SOCKET_ERROR) {
    int last_error = WSAGetLastError();
    if (last_error != WSAEWOULDBLOCK) {
      std::cout << "[!] recvfrom() failed: " << last_error << std::endl;
    }
    return 0;
  } else if (!result) {
    std::cout << "[!] recvfrom() returned 0" << std::endl;
    return 0;
  }

  *from_ip = ntohl(from_address.sin_addr.s_addr);
  *from_port = ntohs(from_address.sin_port);
  return result;
}

internal int platform_win_send_to(Platform_Socket sock, const void *buffer,
                                  int length, int to_ip, int to_port) {
  SOCKADDR_IN to_address;
  to_address.sin_family = AF_INET;
  to_address.sin_addr.s_addr = htonl(to_ip);
  to_address.sin_port = htons(to_port);

  int result = sendto(sock.socket, static_cast<const char *>(buffer), length, 0,
                      (SOCKADDR *)&to_address, sizeof(to_address));

  if (result == SOCKET_ERROR) {
    std::cout << "[!] sendto() failed: " << WSAGetLastError() << std::endl;
    return 0;
  }

  return result;
}

internal LARGE_INTEGER platform_win_wall_clock() {
  LARGE_INTEGER result;
  QueryPerformanceCounter(&result);
  return result;
}

internal float platform_win_elapsed_seconds(Platform_State *state,
                                            LARGE_INTEGER begin,
                                            LARGE_INTEGER end) {
  float result = static_cast<float>(end.QuadPart - begin.QuadPart) /
                 static_cast<float>(state->frequency.QuadPart);
  return result;
}