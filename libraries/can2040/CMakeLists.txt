# libraries/can2040/CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(can2040 LANGUAGES C)

# 1. Define the library and compile its source code file
add_library(can2040 STATIC
    can2040.c
)

# 2. Expose the directory folder path so that bcm_front or driver_door 
# can find #include "can2040.h" cleanly.
# We use 'PUBLIC' so that any application linking to can2040 automatically inherits this include path.
target_include_directories(can2040 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# 3. Link the necessary Pico SDK hardware modules
# can2040 interacts directly with the RP2350's hardware PIO and DMA channels!
target_link_libraries(can2040 PUBLIC
    pico_stdlib
    hardware_pio
    hardware_dma
)