add_library(can_ecu INTERFACE)

# The MAGIC line: This tells CMake that anyone who links to 'kamry_can'
# automatically gets this folder added to their header search paths.
target_include_directories(can_ecu INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})