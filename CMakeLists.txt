cmake_minimum_required(VERSION 3.15)
project(KamryMasterBuild NONE)

include(ExternalProject)

set(GLOBAL_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin")

# 🖥️ TARGET 1: THE NATIVE DESKTOP DASHBOARD
ExternalProject_Add(dash
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/apps/dash"
    CMAKE_ARGS 
        -DBUILD_NATIVE_DASH=ON
        -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${GLOBAL_OUTPUT_DIR}
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/build_dash"
    INSTALL_COMMAND ""
    # 🛠️ FORCE RE-CHECK: Tells CMake to scan for file modifications on every build pass
    STEP_TARGETS build
)
# Force step bypass
ExternalProject_Add_Step(dash force_rebuild
    COMMAND ${CMAKE_COMMAND} -E echo "Checking Dashboard modifications..."
    DEPENDERS configure
    ALWAYS 1
)

# 🧱 TARGET 2: THE EMBEDDED PICO FIRMWARE NODES
ExternalProject_Add(pico
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/apps/pico"
    CMAKE_ARGS 
        -DBUILD_PICO_NODES=ON
        -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
        -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${GLOBAL_OUTPUT_DIR}
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/build_pico"
    INSTALL_COMMAND ""
    USES_TERMINAL_CONFIGURE ON
    USES_TERMINAL_BUILD ON
    STEP_TARGETS build
)
# 🛠️ FORCE RE-CHECK: Tells CMake to scan for your .cpp changes on every build pass
ExternalProject_Add_Step(pico force_rebuild
    COMMAND ${CMAKE_COMMAND} -E echo "Checking Pico node modifications..."
    DEPENDERS configure
    ALWAYS 1
)