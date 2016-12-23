cmake_minimum_required(VERSION 2.8.8)
project(libuipf)

# load CMake modules
include(ExternalProject)

#
# FLAGS
#

# whether to build with OpenCV. This will include types for OpenCV.
option(WITH_OPENCV YES)
option(WITH_TESTS YES)

if(WITH_TESTS)
	enable_testing()
endif()

# find requiered packages
find_package(Qt5Widgets REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# include header files
include_directories("${PROJECT_SOURCE_DIR}/src/include")
include_directories("${PROJECT_BINARY_DIR}")

#c++11 support
#if(UNIX)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -std=gnu++0x")
#endif()

# sanitize memory issues https://gcc.gnu.org/gcc-4.8/changes.html
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")


# TODO properly find Boost
# http://cpprocks.com/using-cmake-to-build-a-cross-platform-project-with-a-boost-dependency/

#
# external dependencies
# http://www.cmake.org/cmake/help/v2.8.8/cmake.html#module%3aExternalProject
#

# yaml-cpp
ExternalProject_Add(YAML_CPP
	GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
	GIT_TAG release-0.5.2
	TEST_COMMAND ""
	INSTALL_COMMAND "" # skip install step for yaml-cpp
)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/YAML_CPP-prefix/src/YAML_CPP/include)
set(YAML_CPP_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/YAML_CPP-prefix/src/YAML_CPP-build/libyaml-cpp.a)

# libuipf
add_library(uipf STATIC

	src/ModuleLoader.cpp
	src/ProcessingChain.cpp

	src/Data.cpp
	src/Module.cpp
	src/ModuleMetaData.cpp

)
target_link_libraries(uipf ${YAML_CPP_LIBRARY})
qt5_use_modules(uipf Core Widgets) # Qt is required for loading modules dynamically
add_dependencies(uipf YAML_CPP)
if (WITH_OPENCV)
	target_compile_definitions(uipf PRIVATE WITH_OPENCV)
	target_link_libraries(uipf opencv_core)
endif()

# libuipf-module
add_library(uipf-module STATIC

	src/Data.cpp
	src/Module.cpp
	src/ModuleMetaData.cpp
)
qt5_use_modules(uipf-module Core Gui) # Qt is required for loading modules dynamically

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

add_executable(test-libuipf-data            test/unit/data.cpp)
add_executable(test-libuipf-module_loader   test/unit/module_loader.cpp)
target_link_libraries(test-libuipf-data             uipf ${Boost_LIBRARIES})
target_link_libraries(test-libuipf-module_loader    uipf uipf-module ${Boost_LIBRARIES})
if (WITH_OPENCV)
	target_compile_definitions(test-libuipf-data PRIVATE WITH_OPENCV)
endif()
add_test(NAME test-libuipf-data          COMMAND test-libuipf-data)
add_test(NAME test-libuipf-module_loader COMMAND test-libuipf-module_loader)


add_subdirectory(test/integration/dataflow)

