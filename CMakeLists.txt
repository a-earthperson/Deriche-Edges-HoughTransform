cmake_minimum_required(VERSION 3.6)
project(DericheHoughTransform)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c src/otsu.h)
add_executable(DericheHoughTransform ${SOURCE_FILES})
target_link_libraries(DericheHoughTransform m)