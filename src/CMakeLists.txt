cmake_minimum_required(VERSION 2.8)


add_executable(reversi_cui
  board.c
  main_cui.c
)
set(EXECUTABLE_OUTPUT_PATH "../")

add_executable(reversi_gui
  board.c
  main_gui.c
)
target_link_libraries(reversi_gui libui)
include_directories("../lib")
set(EXECUTABLE_OUTPUT_PATH "../")

add_executable(reversi_auto
  board.c
  main_auto.c
)

add_executable(reversi_search
  board.c
  search.c
  turns.c
  main_search.c
)
