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

# Find the math library.
find_library(MATH_LIBRARY m)

# If it's found, add it to the executable.
if(MATH_LIBRARY)
    target_link_libraries(cphoto PUBLIC ${MATH_LIBRARY})
endif()