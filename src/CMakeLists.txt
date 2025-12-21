add_executable(ttt_test main.cpp)

add_subdirectory(games)

target_link_libraries(ttt_test PRIVATE tic_tac_toe)
