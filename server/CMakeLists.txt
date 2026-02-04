cmake_minimum_required(VERSION 3.16)
project(WizzManiaServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adjust include_directories / link_libraries depending on how you install Crow.
# For example, if crow.h is in a third_party/crow/include folder in the repo:
# include_directories(${CMAKE_SOURCE_DIR}/third_party/crow/include)
include_directories(/opt/homebrew/opt/crow/include)
include_directories(/opt/homebrew/opt/asio/include)

find_package(SQLite3 REQUIRED)

add_executable(wizzmania_server
    main.cpp
)

target_link_libraries(wizzmania_server PRIVATE SQLite::SQLite3)

