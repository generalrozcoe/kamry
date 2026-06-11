add_library(can INTERFACE)

# The MAGIC line: This tells CMake that anyone who links to 'kamry_can'
# automatically gets this folder added to their header search paths.
target_include_directories(can INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(can INTERFACE 
                                    
                                    can_ecu
                                    can_bcm)