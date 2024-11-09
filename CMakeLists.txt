cmake_minimum_required(VERSION 3.10)
project(CPhoto)

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(cphoto ${SOURCES})