cmake_minimum_required(VERSION 3.16)
project(WizzManiaClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets WebSockets Network)

qt6_standard_project_setup()

set(SOURCES
    main.cpp
    src/AuthClient.cpp
    src/WebSocketClient.cpp
    src/LoginWindow.cpp
    src/ChatWindow.cpp
)

set(HEADERS
    src/AuthClient.h
    src/WebSocketClient.h
    src/LoginWindow.h
    src/ChatWindow.h
)

set(UI_FILES
    ui/LoginWindow.ui
    ui/ChatWindow.ui
)

set(RESOURCE_FILES
    resources.qrc
)

# Set AUTOUIC search paths BEFORE creating the target
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

qt6_add_executable(wizzmania_client ${SOURCES} ${HEADERS} ${UI_FILES})

qt6_add_resources(wizzmania_client RESOURCES ${RESOURCE_FILES})

# Add include directory for generated UI headers
target_include_directories(wizzmania_client PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/wizzmania_client_autogen
)

target_link_libraries(wizzmania_client PRIVATE Qt6::Core Qt6::Widgets Qt6::WebSockets Qt6::Network)
