cmake_minimum_required(VERSION 3.0.0)
project(UIPFGUI)

include_directories("${UIPFGUI_BINARY_DIR}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories("${libuipf_SOURCE_DIR}/src/include")

# enable MOC and UIC for Qt support
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# find Qt library for building the GUI
find_package(Qt5Widgets REQUIRED)


# manually generate ui_headers
QT5_WRAP_UI(UI_HS
    mainwindow.ui
)

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

	RunWorkerThread.cpp

	${RC_SRCS}

	GuiLogger.cpp
	GuiLogger.hpp
	RunControl.cpp RunControl.hpp)

target_link_libraries (uipf-gui boost_graph libuipf)
qt5_use_modules(uipf-gui Core Gui Widgets)


# installation targets
install(TARGETS uipf-gui
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

##install header files
#file(GLOB headerFilesFramework "${CMAKE_CURRENT_SOURCE_DIR}/framework/*.hpp")
#file(GLOB headerFilesTypes "${CMAKE_CURRENT_SOURCE_DIR}/framework/types/*.hpp")
#install(FILES
#	${headerFilesFramework}
#	DESTINATION include/uipf/framework
#)
#install(FILES
#	${headerFilesTypes}
#	DESTINATION include/uipf/framework/types
#)
#install(FILES
#	${CMAKE_CURRENT_SOURCE_DIR}/uipf-gui.desktop
#	${CMAKE_CURRENT_SOURCE_DIR}/uipf.desktop
#	DESTINATION /usr/share/applications
#)
