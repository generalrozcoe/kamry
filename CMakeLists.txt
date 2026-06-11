cmake_minimum_required(VERSION 3.15)
project(KamryProject LANGUAGES CXX)

# 1. Global rules: Force apps into a clean binaries folder and enforce C++17
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/binaries")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Fetch SFML Dependency (Your exact FetchContent setup)
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# 3. Register your reusable code modules first
add_subdirectory(libraries/can_bcm)
add_subdirectory(libraries/can_ecu)
add_subdirectory(libraries/can)


# 4. Register your executable applications last
add_subdirectory(apps/dash)