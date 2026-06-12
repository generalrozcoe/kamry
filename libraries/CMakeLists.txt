# libraries/CMakeLists.txt
cmake_minimum_required(VERSION 3.15)

# 1. Declare all the header-only pieces first so CMake registers their names
add_subdirectory(can_bcm)
add_subdirectory(can)
add_subdirectory(can_ecu)

# 2. Add your compilable C driver last (it can now see all the headers above!)
add_subdirectory(can2040)