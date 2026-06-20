if(NOT BUILD_NATIVE_DASH)
    return() 
endif()

cmake_minimum_required(VERSION 3.15)
project(dash_sim LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)


# Step up two levels to find your shared CAN protocol headers
add_subdirectory(../../libraries/can_bcm ${CMAKE_CURRENT_BINARY_DIR}/can_bcm)
add_subdirectory(../../libraries/can ${CMAKE_CURRENT_BINARY_DIR}/can)
add_subdirectory(../../libraries/can_ecu ${CMAKE_CURRENT_BINARY_DIR}/can_ecu)
add_executable(dash_sim src/main.cpp
                    src/rcan.cpp)
target_include_directories(dash_sim PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(dash_sim PRIVATE can can_bcm can_ecu)

