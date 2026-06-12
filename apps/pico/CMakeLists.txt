if(NOT BUILD_PICO_NODES)
    return() 
endif()

cmake_minimum_required(VERSION 3.15)

# Lock the platform variables specifically inside the app execution thread
set(PICO_PLATFORM rp2350 CACHE STRING "Pico Platform Target" FORCE)
set(PICO_BOARD pico2 CACHE STRING "Pico Board Target" FORCE)

# Define the absolute path tracking to the SDK location
set(PICO_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/pico-sdk" CACHE PATH "Path to Pico SDK" FORCE)

# 🛠️ THE EXACT PATH FIX BASED ON YOUR SIDEBAR: Target the Cortex-M33 GCC compiler configuration!
if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${PICO_SDK_PATH}/cmake/preload/toolchains/pico_arm_cortex_m33_gcc.cmake" CACHE PATH "ARM Toolchain" FORCE)
endif()
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)
project(pico_nodes LANGUAGES C CXX ASM)
set(GLOBAL_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/../../binaries")

pico_sdk_init()

# Add shared sub-project dependencies
add_subdirectory(../../libraries ${CMAKE_CURRENT_BINARY_DIR}/libraries)
# Register all physical hardware microcontroller apps here
add_subdirectory(bcm_front)
# add_subdirectory(driver_door) # Uncomment when you're ready to create this folder!