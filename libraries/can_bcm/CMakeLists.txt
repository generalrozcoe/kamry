cmake_minimum_required(VERSION 3.15)
add_library(can_bcm INTERFACE)

# The MAGIC line: This tells CMake that anyone who links to 'kamry_can'
# automatically gets this folder added to their header search paths.
target_include_directories(can_bcm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
