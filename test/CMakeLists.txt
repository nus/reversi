cmake_minimum_required(VERSION 2.8)

add_executable(tests
  ../src/board.c
  board_test.c
  test.c
)

include_directories("../src")
set(EXECUTABLE_OUTPUT_PATH "../")
