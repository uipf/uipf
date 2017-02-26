cmake_minimum_required(VERSION 2.8.8)
project(libuipf-geomview)

#
# FLAGS
#

option(LIBUIPF_GEOMVIEW_WITH_TESTS "Build test binaries and enable `make test` target" ON)

if(LIBUIPF_GEOMVIEW_WITH_TESTS)
	enable_testing()
endif()

message("libuipf-geomview Build options:")
message("With Tests: ${LIBUIPF_GEOMVIEW_WITH_TESTS}")

#
# find external libraries
#

find_package(CGAL REQUIRED)
message(status "** CGAL Include: ${CGAL_INCLUDE_DIRS}")
message(status "** CGAL Library: ${CGAL_LIBRARY}")
message(status "** CGAL_Core Library: ${CGAL_Core_LIBRARY}")
include_directories(${CGAL_INCLUDE_DIRS})


#
# libraries
#

# libuipf-module
add_library(libuipf-geomview SHARED
	geomview.cpp
)
set_target_properties(libuipf-geomview PROPERTIES
	# CGAL complains if not compiled with this flag
	COMPILE_FLAGS -frounding-math
	# name of lib vs. executeable: https://cmake.org/pipermail/cmake/2007-November/017697.html
	OUTPUT_NAME uipf-geomview
	SOVERSION ${UIPFramework_VERSION_MAJOR}.${UIPFramework_VERSION_MINOR}
	VERSION   ${UIPFramework_VERSION_MAJOR}.${UIPFramework_VERSION_MINOR}.${UIPFramework_VERSION_PATCH}
)

target_link_libraries(libuipf-geomview ${CGAL_LIBRARY} ${CGAL_Core_LIBRARY})


#
# TESTS
#

if(LIBUIPF_GEOMVIEW_WITH_TESTS)
	add_executable(geomview-demo tests/demo.cpp)
	target_link_libraries(geomview-demo    libuipf libuipf-module ${CGAL_LIBRARY} ${CGAL_Core_LIBRARY})

#	add_test(NAME test-libuipf-processing_chain COMMAND test-libuipf-processing_chain)
endif()


# installation targets
install(TARGETS libuipf-geomview
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
install(DIRECTORY ${libuipf-geomview_SOURCE_DIR}/
	DESTINATION include/uipf
	FILES_MATCHING PATTERN "*.hpp"
)
