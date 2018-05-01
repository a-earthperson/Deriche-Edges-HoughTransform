cmake_minimum_required(VERSION 3.6)
project(DericheHoughTransform)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c include/otsu.h bmp.c deriche.c hough.c mat.c otsu.c)
add_executable(DericheHoughTransform ${SOURCE_FILES})
target_link_libraries(DericheHoughTransform m)