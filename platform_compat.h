// platform_compat.h — minimal Windows->POSIX shims for Linux builds
#pragma once

#ifdef _WIN32
  #include <Windows.h>
#else
  // Linux / POSIX
  #include <time.h>
  #include <unistd.h>

  // Provide Sleep(ms) like Windows
  static inline void Sleep(unsigned int ms) {
      struct timespec ts;
      ts.tv_sec  = ms / 1000;
      ts.tv_nsec = (ms % 1000) * 1000000L;
      nanosleep(&ts, nullptr);
  }

  // If code uses ZeroMemory / min / max from Windows.h, give safe stand-ins:
  #ifndef ZeroMemory
  #include <cstring>
  #define ZeroMemory(dst, sz) std::memset((dst), 0, (sz))
  #endif

  // If code uses DWORD, BOOL, etc., provide minimal typedefs:
  #include <cstdint>
  typedef uint32_t DWORD;
  typedef int BOOL;
  #ifndef TRUE
  #define TRUE 1
  #define FALSE 0
  #endif
#endif

