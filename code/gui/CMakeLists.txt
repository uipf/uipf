cmake_minimum_required(VERSION 2.8.8)
project(UIPFGUI)

FindBoost(graph)

include_directories("${UIPFGUI_BINARY_DIR}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# include libuipf headers
include_directories("${libuipf_SOURCE_DIR}/src/include")

# enable MOC and UIC for Qt support
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# find Qt library for building the GUI
find_package(Qt5Widgets REQUIRED)

# manually generate ui_headers
QT5_WRAP_UI(UI_HS mainwindow.ui)

# this command will generate rules that will run rcc on all listed files
# in result RC_SRCS variable will contain paths to files produced by rcc
qt5_add_resources(RC_SRCS mainwindow.qrc)

# add executeable for the gui application
add_executable(uipf-gui

	main-gui.cpp
	MainWindow.cpp
	${UI_HS}

	ImageWindow.cpp

	graph/edge.cpp
	graph/node.cpp
	graph/graphwidget.cpp

	models/ParamsModel.cpp
	models/InputsDelegate.cpp

	RunControl.cpp
	RunWorkerThread.cpp

	${RC_SRCS}

	GuiLogger.cpp
	GuiVisualizationContext.cpp
)

target_link_libraries (uipf-gui boost_graph libuipf opencv_core opencv_imgproc)
qt5_use_modules(uipf-gui Core Gui Widgets)


# installation targets
install(TARGETS uipf-gui RUNTIME DESTINATION bin)

if(UNIX)
	# install desktop files
	include(GNUInstallDirs)

	install(FILES
		${UIPFramework_SOURCE_DIR}/uipf-gui.desktop
		DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications"
		)

	# TODO write manpages
	#	install(FILES
	#		uipf.1
	#		DESTINATION "${CMAKE_INSTALL_MANDIR}/man1"
	#	)
endif()
