set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# 1. Bind compiler paths
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_ASM_COMPILER "/usr/bin/clang")

# 2. Configure architecture parameters for the RP2350 (Pico 2)
set(RP2350_FLAGS "-target arm-none-eabi -march=armv8-m.main+fp -mthumb -mfloat-abi=hard")

# Map your machine local 12.2.1 newlib headers
set(ARM_INCLUDE_FLAGS " \
    -I/usr/include/newlib \
    -I/usr/include/newlib/c++/12.2.1 \
    -I/usr/include/newlib/c++/12.2.1/arm-none-eabi \
")

# 3. Force parameters into global caching
set(CMAKE_C_FLAGS "${RP2350_FLAGS} ${ARM_INCLUDE_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${RP2350_FLAGS} ${ARM_INCLUDE_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_ASM_FLAGS "${RP2350_FLAGS}" CACHE STRING "" FORCE)

# 4. Use the LLVM bare-metal linker
set(CMAKE_EXE_LINKER_FLAGS_INIT "-fuse-ld=lld")

# 5. Skip baseline environment validity checks
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_ASM_COMPILER_WORKS 1)

set(PICO_NO_TARGET_FLAGS 1)