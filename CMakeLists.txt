cmake_minimum_required(VERSION 3.20.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
	cpp20modules
	VERSION 1.0.0
	LANGUAGES CXX
)

add_compile_options(-O3 -m64 -Wall -Wextra -Wpedantic -Wshadow -double-promotion -flto -funroll-loops)

find_package(IPP REQUIRED)

add_subdirectory(executable)
add_subdirectory(libraries)
