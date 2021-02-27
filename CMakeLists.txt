cmake_minimum_required(VERSION 3.17)

project(untitled1)

set(CMAKE_CXX_STANDARD 14)
find_package(Python3 COMPONENTS Development NumPy)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(mandelbrot mandelbrot.cpp matplotlibcpp.h )

target_include_directories(mandelbrot PRIVATE ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})
target_link_libraries(mandelbrot Python3::Python Python3::NumPy )
