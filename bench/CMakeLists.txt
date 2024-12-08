cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")

# Add executables
add_executable(std_array_op array_op.cpp)
add_executable(lni_array_op array_op.cpp)
add_executable(std_insertion insertion.cpp)
add_executable(lni_insertion insertion.cpp)
add_executable(std_back_insertion back_insertion.cpp)
add_executable(lni_back_insertion back_insertion.cpp)
add_executable(std_stack stack.cpp)
add_executable(lni_stack stack.cpp)

# Define macros for USE_LNI_VECTOR
target_compile_definitions(lni_array_op PRIVATE USE_LNI_VECTOR)
target_compile_definitions(lni_insertion PRIVATE USE_LNI_VECTOR)
target_compile_definitions(lni_back_insertion PRIVATE USE_LNI_VECTOR)
target_compile_definitions(lni_stack PRIVATE USE_LNI_VECTOR)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/..)

# Custom clean target
add_custom_target(clean
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_clean.cmake
    COMMENT "Cleaning up"
)

# Generate custom clean script
file(WRITE ${CMAKE_BINARY_DIR}/cmake_clean.cmake
"file(REMOVE \${CMAKE_BINARY_DIR}/std_array_op \${CMAKE_BINARY_DIR}/lni_array_op \${CMAKE_BINARY_DIR}/std_insertion \${CMAKE_BINARY_DIR}/lni_insertion \${CMAKE_BINARY_DIR}/std_back_insertion \${CMAKE_BINARY_DIR}/lni_back_insertion \${CMAKE_BINARY_DIR}/std_stack \${CMAKE_BINARY_DIR}/lni_stack)"
)

# Install rules if needed
install(TARGETS std_array_op lni_array_op std_insertion lni_insertion std_back_insertion lni_back_insertion std_stack lni_stack DESTINATION bin)
