file(GLOB SRC_FILES src/*.cc)

add_library(benchmark_lib STATIC ${SRC_FILES})

target_include_directories(benchmark_lib PUBLIC src)

target_link_libraries(
	benchmark_lib
	PUBLIC
	OpenMP::OpenMP_CXX
	IPP::ippcore IPP::ipps
	benchmark::benchmark
)