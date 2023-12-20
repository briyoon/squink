cmake_minimum_required(VERSION 3.25)

# Set the project name and version
project(Squink VERSION 0.1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add include directory
include_directories(include)

# Create a library from src/squink.c
add_library(squink_lib STATIC src/squink.c)
set_target_properties(squink_lib PROPERTIES PUBLIC_HEADER include/squink.h)

# Collect all .c files in the test directory
file(GLOB TEST_SOURCES "test/*.c")

# Create an executable for each test source file
foreach(TEST_SOURCE ${TEST_SOURCES})
    # Get a valid name for each test executable based on the source file name
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    # Add executable for each test file
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    # Link the test executable with the squink_lib library
    target_link_libraries(${TEST_NAME} squink_lib)
endforeach()
