cmake_minimum_required(VERSION 3.0.0)
project(UIPFramework)

#
# FLAGS
#

option(WITH_TESTS "Build test binaries and enable `make test` target" ON)
option(WITH_OPENCV "Build with openCV, enable openCV types" OFF)

message("Build options:")
message("With Tests: ${WITH_TESTS}")
message("With OpenCV: ${WITH_OPENCV}")

set(LIBUIPF_WITH_TESTS ${WITH_TESTS})
set(LIBUIPF_WITH_OPENCV ${WITH_OPENCV})

if(WITH_TESTS)
	enable_testing()
endif()

#c++11 support
if(${CMAKE_VERSION} VERSION_LESS 3.1.0)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
else()
	set(CMAKE_CXX_STANDARD 11)
endif()

if(UNIX)
	#c++11 support
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

	if(CMAKE_BUILD_TYPE MATCHES Debug)
		# sanitize memory issues https://gcc.gnu.org/gcc-4.8/changes.html
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
	else()
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
	endif()
endif()


# project version
set(UIPFramework_VERSION_MAJOR 2)
set(UIPFramework_VERSION_MINOR 0)
set(UIPFramework_VERSION_PATCH 0)
set(UIPFramework_VERSION "${UIPFramework_VERSION_MAJOR}.${UIPFramework_VERSION_MINOR}.${UIPFramework_VERSION_PATCH}-alpha")
configure_file (
		"${PROJECT_SOURCE_DIR}/uipf.hpp.in"
		"${PROJECT_BINARY_DIR}/uipf.hpp"
)


add_subdirectory(libuipf)

add_subdirectory(modules)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
add_subdirectory(console)
#add_subdirectory(gui)


##
## installation targets
##

#install(TARGETS libuipf uipf-module
#        LIBRARY DESTINATION lib
#		ARCHIVE DESTINATION lib
#)
#install(TARGETS uipf uipf-gui
#		RUNTIME DESTINATION bin
#)
#install(TARGETS
#			AddListItemModule
#			ConvolutionModule
#			DummyModule
#			GaussianModule
#			LoadImageModule
#			LongRunningDummyModule
#			MergeChannelsModule
#			SelectListItemModule
#			ShowImageListModule
#			ShowImageModule
#			SplitChannelsModule
#			StoreImageModule
#
#        LIBRARY DESTINATION lib/uipf
#)

#install header files
#file(GLOB headerFilesUipf "${libuipf_SOURCE_DIR}/src/include/*.hpp")
#file(GLOB headerFilesData "${libuipf_SOURCE_DIR}/src/include/data/*.hpp")
#install(FILES
#	${headerFilesUipf}
#	DESTINATION include/uipf
#)
#install(FILES
#	${headerFilesData}
#	DESTINATION include/uipf/data
#)
#install(FILES
#	${CMAKE_CURRENT_SOURCE_DIR}/uipf-gui.desktop
#	${CMAKE_CURRENT_SOURCE_DIR}/uipf.desktop
#	DESTINATION /usr/share/applications
#)

