cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")

if (MSVC)
    add_compile_options(/wd5045)  # Disable C5045 warnings
    add_compile_options(/wd4514)  # Disable C4514 warnings
endif()

# Add executables
add_executable(tester vector.cpp tester.cpp)
add_executable(mytester vector.cpp mytester.cpp)

# Define DEBUG target
add_executable(dbg vector.cpp tester.cpp)
target_compile_definitions(dbg PRIVATE DEBUG)

# Custom clean target
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_clean.cmake
    COMMENT "Cleaning up"
)

# Generate custom clean script
file(WRITE ${CMAKE_BINARY_DIR}/cmake_clean.cmake
"file(REMOVE \${CMAKE_BINARY_DIR}/tester \${CMAKE_BINARY_DIR}/mytester \${CMAKE_BINARY_DIR}/dbg)"
)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Install rules if needed
install(TARGETS tester mytester dbg DESTINATION bin)
