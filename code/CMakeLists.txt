cmake_minimum_required(VERSION 2.8.8)
project(UIPFramework)

set(CMAKE_MODULE_PATH ${UIPFramework_SOURCE_DIR}/cmake)
include(cmake/BoostHelper.cmake)

#
# FLAGS
#

option(WITH_TESTS "Build test binaries and enable `make test` target" ON)
option(WITH_OPENCV "Build with openCV, enable openCV types" ON)
option(WITH_GUI "Build with graphical user interface (requires Qt)" ON)
option(WITH_MODULES "Build with basic modules" ON)

message("Build options:")
message("With Tests: ${WITH_TESTS}")
message("With OpenCV: ${WITH_OPENCV}")
message("With GUI: ${WITH_GUI}")

set(LIBUIPF_WITH_TESTS ${WITH_TESTS})
set(LIBUIPF_WITH_OPENCV ${WITH_OPENCV})

if(WITH_TESTS)
	enable_testing()
endif()

# c++11 support
if(${CMAKE_VERSION} VERSION_LESS 3.1.0)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
else()
	set(CMAKE_CXX_STANDARD 11)
endif()

if(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

	# sanitize memory issues https://gcc.gnu.org/gcc-4.8/changes.html
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
endif()

message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message("CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

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
# TODO extract libuipf-module from libuipf
add_subdirectory(libuipf-geomview)


include_directories("${UIPFramework_BINARY_DIR}")
configure_file(paths.cpp.in ${UIPFramework_BINARY_DIR}/paths.cpp)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
add_subdirectory(console)

if (WITH_GUI)
	add_subdirectory(gui)
endif()
if (WITH_MODULES)
	add_subdirectory(modules)
endif()
