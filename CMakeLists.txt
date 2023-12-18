cmake_minimum_required(VERSION 3.25)

# Set the project name and version
project(Squink VERSION 0.1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add include directory
include_directories(include)

# Add the executable
add_executable(squink src/log.c)

# Specify the public headers
set_target_properties(squink PROPERTIES PUBLIC_HEADER include/squink.h)

# Set the location for library installation
# install(TARGETS squink
#     ARCHIVE DESTINATION lib
#     LIBRARY DESTINATION lib
#     RUNTIME DESTINATION bin
#     PUBLIC_HEADER DESTINATION include)
