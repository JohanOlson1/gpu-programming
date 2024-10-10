cmake_minimum_required(VERSION 3.20.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER "/opt/rh/gcc-toolset-12/root/bin/g++")

project(
	gpu_prog
	VERSION 1.0.0
	LANGUAGES CXX
)

add_compile_options(-g -m64 -O3 -Wall -Wextra -Wpedantic -Wshadow -Wdouble-promotion -funroll-loops -fopenmp -foffload=nvptx-none -foffload=-lm)

find_package(OpenMP REQUIRED)
find_package(IPP REQUIRED)
find_package(MKL REQUIRED)
find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

add_subdirectory(executable)
add_subdirectory(libraries)
