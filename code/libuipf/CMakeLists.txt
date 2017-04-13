cmake_minimum_required(VERSION 2.8.8)
project(libuipf)

#
# FLAGS
#

option(LIBUIPF_WITH_TESTS "Build test binaries and enable `make test` target" ON)
option(LIBUIPF_WITH_OPENCV "Build with openCV, enable openCV types" OFF)

if(LIBUIPF_WITH_TESTS)
	enable_testing()
endif()

message("libuipf Build options:")
message("With Tests: ${LIBUIPF_WITH_TESTS}")
message("With OpenCV: ${LIBUIPF_WITH_OPENCV}")


# include header files
include_directories("${libuipf_SOURCE_DIR}/src/include")


#
# find external libraries
#

# load CMake modules
include(ExternalProject)
include(FindPkgConfig)

# glibmm (c++ wrapper for glib2)
pkg_search_module (GLIBMM REQUIRED glibmm-2.4)
include_directories(${GLIBMM_INCLUDE_DIRS})
link_directories(${GLIBMM_LIBRARY_DIRS})

# find Boost
if(LIBUIPF_WITH_TESTS)
	FindBoost(filesystem unit_test_framework system)
else()
	FindBoost(filesystem system)
endif()

# yaml-cpp
# http://www.cmake.org/cmake/help/v2.8.8/cmake.html#module%3aExternalProject
ExternalProject_Add(YAML_CPP
	GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
	GIT_TAG release-0.5.2
	CMAKE_ARGS -DYAML_CPP_BUILD_TOOLS=OFF -DYAML_CPP_BUILD_CONTRIB=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=ON
	TEST_COMMAND ""
	INSTALL_COMMAND "" # skip install step for yaml-cpp
)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/YAML_CPP-prefix/src/YAML_CPP/include)
set(YAML_CPP_LIBRARY_FILE "${CMAKE_CURRENT_BINARY_DIR}/YAML_CPP-prefix/src/YAML_CPP-build/libyaml-cpp.a")
set(YAML_CPP_LIBRARY "-Wl,--whole-archive ${YAML_CPP_LIBRARY_FILE} -Wl,--no-whole-archive")


#
# libraries
#

# libuipf-module
add_library(libuipf-module SHARED

	src/ModuleMetaData.cpp
	src/ModuleInterface.cpp
	src/data/opencv.cpp
	src/util.cpp
)
set_target_properties(libuipf-module PROPERTIES
	# name of lib vs. executeable: https://cmake.org/pipermail/cmake/2007-November/017697.html
	OUTPUT_NAME uipf-module
	SOVERSION ${UIPFramework_VERSION_MAJOR}.${UIPFramework_VERSION_MINOR}
	VERSION   ${UIPFramework_VERSION_MAJOR}.${UIPFramework_VERSION_MINOR}.${UIPFramework_VERSION_PATCH}
)
if (LIBUIPF_WITH_OPENCV)
	target_compile_definitions(libuipf-module PRIVATE WITH_OPENCV)
	target_link_libraries(libuipf-module opencv_core opencv_highgui)
	# boost filesystem is also only needed if compiled with opencv
	target_link_libraries(libuipf-module ${Boost_LIBRARY_DIRS}/libboost_filesystem.so)
endif()

# libuipf
add_library(libuipf SHARED

	src/ModuleLoader.cpp
	src/ProcessingChain.cpp
	src/Runner.cpp
	src/util.cpp
	src/logging.cpp
)
set_target_properties(libuipf PROPERTIES
	# name of lib vs. executeable: https://cmake.org/pipermail/cmake/2007-November/017697.html
	OUTPUT_NAME uipf
	SOVERSION ${UIPFramework_VERSION_MAJOR}.${UIPFramework_VERSION_MINOR}
	VERSION   ${UIPFramework_VERSION_MAJOR}.${UIPFramework_VERSION_MINOR}.${UIPFramework_VERSION_PATCH}
)
target_link_libraries(libuipf libuipf-module ${YAML_CPP_LIBRARY} ${GLIBMM_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(libuipf YAML_CPP)


#
# TESTS
#

if(LIBUIPF_WITH_TESTS)
	add_executable(test-libuipf-data             test/unit/data.cpp)
	if (LIBUIPF_WITH_OPENCV)
		target_compile_definitions(test-libuipf-data PRIVATE WITH_OPENCV)
	endif()
	add_executable(test-libuipf-module_loader    test/unit/module_loader.cpp)
	add_executable(test-libuipf-processing_chain test/unit/processing_chain.cpp)
	target_link_libraries(test-libuipf-data             libuipf ${Boost_LIBRARIES})
	target_link_libraries(test-libuipf-module_loader    libuipf libuipf-module ${Boost_LIBRARIES})
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


# installation targets
install(TARGETS libuipf libuipf-module
    LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
#install(FILES ${YAML_CPP_LIBRARY_FILE}
#	DESTINATION lib
#	RENAME libuipf-yaml-cpp.a
#)
if(LIBUIPF_WITH_OPENCV)
	install(DIRECTORY ${libuipf_SOURCE_DIR}/src/include/
			DESTINATION include/uipf
			FILES_MATCHING PATTERN "*.hpp"
	)
else()
	install(DIRECTORY ${libuipf_SOURCE_DIR}/src/include/
			DESTINATION include/uipf
			FILES_MATCHING PATTERN "*.hpp"
			PATTERN opencv.hpp EXCLUDE
	)
endif()
