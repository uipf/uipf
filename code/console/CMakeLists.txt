cmake_minimum_required(VERSION 2.8.8)
project(uipf)

#
# find external libraries
#

# find Boost
# http://cpprocks.com/using-cmake-to-build-a-cross-platform-project-with-a-boost-dependency/
find_package(Boost 1.55 COMPONENTS program_options REQUIRED)
message(status "** Boost Console Include: ${Boost_INCLUDE_DIR}")
message(status "** Boost Console Libraries: ${Boost_LIBRARY_DIRS}")
message(status "** Boost Console Libraries: ${Boost_LIBRARIES}")
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

# include libuipf headers
include_directories("${libuipf_SOURCE_DIR}/src/include")
include_directories("${libuipf_SOURCE_DIR}/src")

# add one executeable for the console application
add_executable(uipf
	main.cpp
)
target_link_libraries(uipf libuipf boost_program_options)


# installation targets
install(TARGETS uipf RUNTIME DESTINATION bin)
