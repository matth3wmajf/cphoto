# Specify the minimum version of CMake that's required in order to generate the
# build file(s).
cmake_minimum_required(VERSION 3.10)

# Specify the project name.
project(cphoto)

# Where is our source code?
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c")

# Where are our headers?
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

# What's the name of the executable and where's the source code that we should
# compile into this executable?
add_executable(cphoto ${SOURCES})

# We need the C math library. It's vital.
target_link_libraries(cphoto m)