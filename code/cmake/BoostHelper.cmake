
macro(FindBoost components)

	# find Boost
	# http://cpprocks.com/using-cmake-to-build-a-cross-platform-project-with-a-boost-dependency/
	find_package(Boost 1.54 COMPONENTS ${components} REQUIRED)
	message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
	message(status "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
	message(status "** Boost Libraries: ${Boost_LIBRARIES}")
	include_directories(${Boost_INCLUDE_DIR})
	link_directories(${Boost_LIBRARY_DIRS})

endmacro()