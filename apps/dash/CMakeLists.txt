include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)


add_executable(dash src/main.cpp
					src/gauge.cpp
					src/common.cpp
					src/rcan.cpp
					src/PID.cpp)
target_include_directories(dash PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(dash PRIVATE SFML::Graphics
                                    
                                    can)
target_compile_features(dash PRIVATE cxx_std_17)
target_link_libraries(dash PRIVATE SFML::Graphics )
