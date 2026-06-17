cmake_minimum_required(VERSION 3.15)

# 1. Declare the binary executable name and its source files
add_executable(bcm_rear src/main.cpp)

# 2. Link your hardware modules and libraries
target_link_libraries(bcm_rear PRIVATE 
    can        
    can2040        
    pico_stdlib
    hardware_spi    
)

# 3. Hook binary into the Pico SDK core generation matrix
pico_add_extra_outputs(bcm_rear)