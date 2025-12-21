add_library(tic_tac_toe tic_tac_toe.cpp tic_tac_toe_state.cpp)

target_include_directories(tic_tac_toe PUBLIC ${PROJECT_SOURCE_DIR}/include)
