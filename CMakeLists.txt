cmake_minimum_required(VERSION 3.10)

project(clox LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STADARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(clox
  src/main.c
  src/chunk.c
  src/memory.c
  src/debug.c
  src/value.c
)
target_include_directories(clox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)


# ------ testing
enable_testing()
add_executable(test_clox
  test/test_clox.c
  src/chunk.c
  src/memory.c
  src/debug.c
  src/value.c
)
target_include_directories(test_clox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_test(NAME clox_test COMMAND $<TARGET_FILE:test_clox>)
