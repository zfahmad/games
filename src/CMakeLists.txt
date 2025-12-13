file(GLOB games_source_files
    "*.cpp"
    "*.h")

add_library(games ${games_source_files})
