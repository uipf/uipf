cmake_minimum_required(VERSION 2.8.8)
project(uipf-modules)


# find Boost
# http://cpprocks.com/using-cmake-to-build-a-cross-platform-project-with-a-boost-dependency/
#find_package(Boost 1.55 COMPONENTS filesystem system REQUIRED)
#message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
#message(status "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
#message(status "** Boost Libraries: ${Boost_LIBRARIES}")
#include_directories(${Boost_INCLUDE_DIR})
#link_directories(${Boost_LIBRARY_DIRS})
#

include_directories("${libuipf_SOURCE_DIR}/src/include")

#
# modules
#

if(WITH_OPENCV)
	add_library(LoadImageModule SHARED io/LoadImageModule.cpp)
	target_link_libraries(LoadImageModule libuipf-module opencv_core opencv_highgui)

	add_library(LoadImagesModule SHARED io/LoadImagesModule.cpp)
	target_link_libraries(LoadImagesModule libuipf-module opencv_core opencv_highgui ${Boost_LIBRARIES})

	# installation targets
	install(TARGETS
		LoadImageModule
		LoadImagesModule


		LIBRARY DESTINATION lib/uipf
		ARCHIVE DESTINATION lib/uipf
	)
endif()
