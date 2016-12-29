cmake_minimum_required(VERSION 2.8.8)
project(libuipf)

#
# FLAGS
#

# whether to build with OpenCV. This will include types for OpenCV.
option(WITH_TESTS "Build test binaries and enable `make test` target" ON)
option(WITH_OPENCV "Build with openCV, enable openCV types" OFF)

if(WITH_TESTS)
	enable_testing()
endif()

message("Build options:")
message("With Tests: ${WITH_TESTS}")
message("With OpenCV: ${WITH_OPENCV}")

# load CMake modules
include(ExternalProject)
include(FindPkgConfig)


# include header files
include_directories("${libuipf_SOURCE_DIR}/src/include")
include_directories("${libuipf_SOURCE_DIR}/src")


#
# find external libraries
#

# glibmm (c++ wrapper for glib2
#pkg_search_module (GLIB2  REQUIRED glib-2.0)
pkg_search_module (GLIBMM REQUIRED glibmm-2.4)
include_directories(${GLIBMM_INCLUDE_DIRS})
link_directories(${GLIBMM_LIBRARY_DIRS})

# find Boost
# http://cpprocks.com/using-cmake-to-build-a-cross-platform-project-with-a-boost-dependency/
find_package(Boost 1.55 COMPONENTS filesystem unit_test_framework system REQUIRED)
message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
message(status "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
message(status "** Boost Libraries: ${Boost_LIBRARIES}")
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

# yaml-cpp
# http://www.cmake.org/cmake/help/v2.8.8/cmake.html#module%3aExternalProject
ExternalProject_Add(YAML_CPP
	GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
	GIT_TAG release-0.5.2
	CMAKE_ARGS -DYAML_CPP_BUILD_TOOLS=OFF -DYAML_CPP_BUILD_CONTRIB=OFF
	TEST_COMMAND ""
	INSTALL_COMMAND "" # skip install step for yaml-cpp
)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/YAML_CPP-prefix/src/YAML_CPP/include)
set(YAML_CPP_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/YAML_CPP-prefix/src/YAML_CPP-build/libyaml-cpp.a)


#
# libraries
#

# libuipf-module
add_library(uipf-module STATIC

	src/Data.cpp
	src/Module.cpp
	src/ModuleMetaData.cpp
)

# libuipf
add_library(libuipf STATIC

	src/ModuleLoader.cpp
	src/ProcessingChain.cpp
#	src/Runner.cpp
	src/util.cpp
)
# name of lib vs. executeable: https://cmake.org/pipermail/cmake/2007-November/017697.html
set_target_properties(libuipf PROPERTIES OUTPUT_NAME uipf)
target_link_libraries(libuipf uipf-module ${YAML_CPP_LIBRARY} ${GLIBMM_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(libuipf YAML_CPP)
if (WITH_OPENCV)
	target_compile_definitions(libuipf PRIVATE WITH_OPENCV)
	target_link_libraries(libuipf opencv_core)
endif()


#
# TESTS
#

if(WITH_TESTS)
	add_executable(test-libuipf-data             test/unit/data.cpp)
	if (WITH_OPENCV)
		target_compile_definitions(test-libuipf-data PRIVATE WITH_OPENCV)
	endif()
	add_executable(test-libuipf-module_loader    test/unit/module_loader.cpp)
	add_executable(test-libuipf-processing_chain test/unit/processing_chain.cpp)
	target_link_libraries(test-libuipf-data             libuipf ${Boost_LIBRARIES})
	target_link_libraries(test-libuipf-module_loader    libuipf uipf-module ${Boost_LIBRARIES})
	target_link_libraries(test-libuipf-processing_chain libuipf ${Boost_LIBRARIES})
	add_test(NAME test-libuipf-data             COMMAND test-libuipf-data)
	add_test(NAME test-libuipf-module_loader    COMMAND test-libuipf-module_loader)
	add_test(NAME test-libuipf-processing_chain COMMAND test-libuipf-processing_chain)

	add_subdirectory(test/integration/dataflow)

	if(UNIX)
		# run make for custom module using GCC
		configure_file("${libuipf_SOURCE_DIR}/test/integration/compiler-gcc/Makefile"       "${libuipf_BINARY_DIR}/test/integration/compiler-gcc/Makefile")
		configure_file("${libuipf_SOURCE_DIR}/test/integration/compiler-gcc/GccModule.cpp"  "${libuipf_BINARY_DIR}/test/integration/compiler-gcc/GccModule.cpp")
		add_custom_target(test-compiler-gcc-build ALL make
				DEPENDS uipf "${libuipf_BINARY_DIR}/test/integration/compiler-gcc/GccModule.cpp" "${libuipf_BINARY_DIR}/test/integration/compiler-gcc/Makefile"
				WORKING_DIRECTORY ${libuipf_BINARY_DIR}/test/integration/compiler-gcc
				VERBATIM
		)
		# run make for custom module using Clang
		configure_file("${libuipf_SOURCE_DIR}/test/integration/compiler-clang/Makefile"        "${libuipf_BINARY_DIR}/test/integration/compiler-clang/Makefile")
		configure_file("${libuipf_SOURCE_DIR}/test/integration/compiler-clang/ClangModule.cpp" "${libuipf_BINARY_DIR}/test/integration/compiler-clang/ClangModule.cpp")
		add_custom_target(test-compiler-clang-build ALL make
				DEPENDS uipf "${libuipf_BINARY_DIR}/test/integration/compiler-clang/ClangModule.cpp" "${libuipf_BINARY_DIR}/test/integration/compiler-clang/Makefile"
				WORKING_DIRECTORY ${libuipf_BINARY_DIR}/test/integration/compiler-clang
				VERBATIM
		)
	endif()
endif()
