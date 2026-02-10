cmake_minimum_required(VERSION 3.16)
project(WizzManiaServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Try local third_party first, then fallback to system/Homebrew paths
if(EXISTS "${CMAKE_SOURCE_DIR}/third_party/Crow/include")
    include_directories(${CMAKE_SOURCE_DIR}/third_party/Crow/include)
    message(STATUS "Using local Crow from third_party")
else()
    include_directories(/opt/homebrew/opt/crow/include)
    message(STATUS "Using system/Homebrew Crow")
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/third_party/asio/include")
    include_directories(${CMAKE_SOURCE_DIR}/third_party/asio/include)
    message(STATUS "Using local ASIO from third_party")
else()
    include_directories(/opt/homebrew/opt/asio/include)
    message(STATUS "Using system/Homebrew ASIO")
endif()

find_package(SQLite3 REQUIRED)

add_executable(wizzmania_server
    main.cpp
)

target_link_libraries(wizzmania_server PRIVATE SQLite::SQLite3)

