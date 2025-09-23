cmake_minimum_required(VERSION 3.10)

project(clox LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STADARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(clox
  main.c
  chunk.c
  memory.c
  debug.c
  value.c
)

target_include_directories(clox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
