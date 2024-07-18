file(GLOB SRC_FILES src/*.cc)

add_library(library1 STATIC ${SRC_FILES})

target_include_directories(library1 PUBLIC src)

target_link_libraries(
	library1
	PUBLIC
	IPP::ippcore IPP::ipps
)