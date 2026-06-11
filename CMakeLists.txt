cmake_minimum_required(VERSION 3.28)
project(KamryEmbeddedSystems LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)
set(cmake_cxx_standard_required on)
add_subdirectory(include)
add_subdirectory(apps/dash)