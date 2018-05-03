cmake_minimum_required(VERSION 3.6)
project(DericheHoughTransform)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES src/main.c src/bmp.c src/deriche.c src/hough.c src/mat.c src/otsu.c)
add_executable(DericheHoughTransform ${SOURCE_FILES})
target_link_libraries(DericheHoughTransform m)
