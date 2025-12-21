cmake_minimum_required(VERSION 3.15)
project(games)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(src)

# add_executable(main src/main.cpp)
# target_include_directories(tic_tac_toe PUBLIC src)
# target_link_libraries(main PRIVATE tic_tac_toe)
