cmake_minimum_required(VERSION 2.8.8)
project(uipf)

# find external libraries
FindBoost(program_options)

# include libuipf headers
include_directories("${libuipf_SOURCE_DIR}/src/include")

# add one executeable for the console application
add_executable(uipf
	main.cpp
)
target_link_libraries(uipf libuipf boost_program_options)

# installation targets
install(TARGETS uipf RUNTIME DESTINATION bin)

if(UNIX)
	# install desktop files
	include(GNUInstallDirs)

	install(FILES
		${UIPFramework_SOURCE_DIR}/uipf.desktop
		DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications"
	)

	# TODO write manpages
	#	install(FILES
	#		uipf.1
	#		DESTINATION "${CMAKE_INSTALL_MANDIR}/man1"
	#	)
endif()
