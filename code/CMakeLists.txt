cmake_minimum_required(VERSION 2.8.8)
project(UIPFramework)


if(UNIX)
	#c++11 support
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

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
