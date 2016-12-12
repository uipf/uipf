cmake_minimum_required(VERSION 2.8.8)
include(ExternalProject)

project(libuipf)


#
# FLAGS
#

# whether to build with OpenCV. This will include types for OpenCV.
set(WITH_OPENCV yes)




enable_testing()


include_directories(src/include)

#c++11 support
#if(UNIX)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -std=gnu++0x")
#endif()

# sanitize memory issues https://gcc.gnu.org/gcc-4.8/changes.html
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")



add_library(uipf STATIC

	src/Data.cpp
	src/Module.cpp
)
if (WITH_OPENCV)
	target_compile_definitions(uipf PRIVATE WITH_OPENCV)
	target_link_libraries(uipf opencv_core)
endif()
#qt5_use_modules(uipf-framework Core Widgets) # Qt is required for loading modules dynamically
#add_dependencies(uipf-framework YAML_CPP)

#
# TESTS
#

include_directories(/usr/local/include)

# find boost
# https://cmake.org/cmake/help/v3.0/module/FindBoost.html
#set(Boost_USE_STATIC_LIBS	ON) # only find static libs
#set(Boost_NO_BOOST_CMAKE	OFF)
#find_package(Boost 1.62.0
#	COMPONENTS system unit_test_framework
#	PATHS /usr/local
#	REQUIRED
#)
#if(Boost_FOUND)
#	message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
#	message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
#	message(STATUS "Boost_VERSION: ${Boost_VERSION}")
#	include_directories(${Boost_INCLUDE_DIRS})
#endif()

add_executable(test-libuipf
		test/unit/libuipf/data.cpp
)
target_link_libraries(test-libuipf uipf ${Boost_LIBRARIES})
if (WITH_OPENCV)
	target_compile_definitions(test-libuipf PRIVATE WITH_OPENCV)
endif()
add_test(NAME test-libuipf COMMAND test-libuipf)


# ref:
# http://cpprocks.com/using-cmake-to-build-a-cross-platform-project-with-a-boost-dependency/