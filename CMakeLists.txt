cmake_minimum_required(VERSION 3.20.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
	cpp20modules
	VERSION 1.0.0
	LANGUAGES CXX
)

add_compile_options(-g -O3 -Wall -Wextra -Wpedantic -Wshadow -Wdouble-promotion -flto -funroll-loops -fopenmp)

find_package(OpenMP REQUIRED)
find_package(IPP REQUIRED)
find_package(MKL REQUIRED)
find_package(benchmark REQUIRED)

add_subdirectory(executable)
add_subdirectory(libraries)
