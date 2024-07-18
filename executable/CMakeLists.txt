add_executable(executable src/main.cc)

target_include_directories(executable PUBLIC src)

target_compile_options(executable PUBLIC)

target_link_libraries(
	executable
	PRIVATE
	benchmark_lib
	benchmark::benchmark
)
