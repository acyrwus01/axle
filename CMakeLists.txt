cmake_minimum_required(VERSION 3.15)
project(DBG)
# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-Wall -Wextra -pedantic -Werror)
option(ENABLE_PCH "Enable Precompiled Headers" OFF)


list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

set(HEADERS
  src/log/ConsoleLoggerHandler.h
  src/log/DebugLogger.h
  src/log/FileLoggerHandler.h
  src/log/LoggerHandler.h
)
set(SOURCES
  src/log/main.cpp
)

add_executable(DBG ${SOURCES})

# if(ENABLE_PCH)
#   target_precompile_headers(DBG PRIVATE <vector> <string> <map>)
# endif()

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
endif()
