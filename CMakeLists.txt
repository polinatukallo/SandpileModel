cmake_minimum_required(VERSION 3.10)
project(sandmodel)

set(CMAKE_CXX_STANDARD 17) 

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories(sp)

add_executable(sandmodel
    sp/main.cpp
    sp/sandpile.cpp
    sp/bmp.cpp
    sp/que.cpp
)

set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/output")
file(MAKE_DIRECTORY ${OUTPUT_DIR})
