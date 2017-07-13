cmake_minimum_required(VERSION 2.8)

add_executable(tests
  ../src/board.c
  ../src/turns.c
  board_test.c
  turns_test.c
  test.c
)

add_executable(benchmarks
  ../src/board.c
  ../src/turns.c
  ../src/search.c
  benchmark.c
)

include_directories("../src")
set(EXECUTABLE_OUTPUT_PATH "../")
