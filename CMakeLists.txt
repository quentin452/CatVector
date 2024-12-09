cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 23)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_CONFIGURATION_TYPES MATCHES "Debug")
    add_compile_definitions(DEBUG) 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall") 
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")
endif()

if (MSVC)
    add_compile_options(/wd5045)  # Disable C5045 warnings
    add_compile_options(/wd4514)  # Disable C4514 warnings
endif()

# Add executables
add_executable(tester vector.cpp tester.cpp)

# Custom clean target
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_clean.cmake
    COMMENT "Cleaning up"
)

# Generate custom clean script
file(WRITE ${CMAKE_BINARY_DIR}/cmake_clean.cmake
"file(REMOVE \${CMAKE_BINARY_DIR}/tester)"
)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Install rules if needed
install(TARGETS tester DESTINATION bin)
