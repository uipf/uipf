cmake_minimum_required(VERSION 2.8.8)
project(uipf-modules)

include_directories("${libuipf_SOURCE_DIR}/src/include")

#
# modules
#

add_library(LoadImageModule SHARED io/LoadImageModule.cpp)
target_link_libraries(LoadImageModule uipf-module opencv_core opencv_highgui)

# installation targets
install(TARGETS
	LoadImageModule


	LIBRARY DESTINATION lib/uipf
	ARCHIVE DESTINATION lib/uipf
)
