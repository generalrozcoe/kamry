if(NOT BUILD_NATIVE_DASH)
    return() 
endif()

cmake_minimum_required(VERSION 3.15)
project(dash LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/binaries")
set(CMAKE_CXX_STANDARD 17)

# Fetch SFML for desktop rendering
include(FetchContent)
FetchContent_Declare(SFML
URL https://github.com/SFML/SFML/archive/refs/tags/3.0.2.zip    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Step up two levels to find your shared CAN protocol headers
add_subdirectory(../../libraries/can_bcm ${CMAKE_CURRENT_BINARY_DIR}/can_bcm)
add_subdirectory(../../libraries/can ${CMAKE_CURRENT_BINARY_DIR}/can)
add_subdirectory(../../libraries/can_ecu ${CMAKE_CURRENT_BINARY_DIR}/can_ecu)
add_executable(dash src/main.cpp
                    src/gauge.cpp
                    src/common.cpp
                    src/PID.cpp
                    src/rcan.cpp)
target_include_directories(dash PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(dash PRIVATE sfml-graphics can can_bcm can_ecu)