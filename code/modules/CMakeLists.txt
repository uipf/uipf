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
	# imgproc modules
	add_library(ResizeModule SHARED improc/ResizeModule.cpp)
	target_link_libraries(ResizeModule libuipf-module opencv_core opencv_highgui opencv_imgproc)

	# io modules
	add_library(LoadImageModule SHARED io/LoadImageModule.cpp)
	target_link_libraries(LoadImageModule libuipf-module opencv_core opencv_highgui)

	add_library(LoadImagesModule SHARED io/LoadImagesModule.cpp)
	target_link_libraries(LoadImagesModule libuipf-module opencv_core opencv_highgui ${Boost_LIBRARIES})

	add_library(StoreImageModule SHARED io/StoreImageModule.cpp)
	target_link_libraries(StoreImageModule libuipf-module opencv_core opencv_highgui)

	# installation targets
	install(TARGETS
		# imgproc modules
		ResizeModule

		# io modules
		LoadImageModule
		LoadImagesModule
		StoreImageModule

		LIBRARY DESTINATION lib/uipf
		ARCHIVE DESTINATION lib/uipf
	)


	# module tests
	file(COPY tests DESTINATION ${uipf-modules_BINARY_DIR}
		USE_SOURCE_PERMISSIONS
		FILES_MATCHING REGEX ".*\\.(yml|jpg)"
	)
	configure_file(${uipf-modules_SOURCE_DIR}/tests/improc_resize.sh
		           ${uipf-modules_BINARY_DIR}/tests/improc_resize.sh
	)
	add_test(NAME module_improc_resize COMMAND ${uipf-modules_BINARY_DIR}/tests/improc_resize.sh WORKING_DIRECTORY ${uipf-modules_BINARY_DIR}/tests)

endif()
