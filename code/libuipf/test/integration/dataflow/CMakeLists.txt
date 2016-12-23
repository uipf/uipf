

find_package(Qt5Widgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# make sure MOC will find UIPF
SET(CMAKE_AUTOMOC_MOC_OPTIONS
	-I${libuipf_BINARY_DIR}
)
MESSAGE(${libuipf_BINARY_DIR})

add_library(TestProducerModule SHARED ProducerModule.cpp)
qt5_use_modules(TestProducerModule Core ) #QtCore is needed for <QPlugin>
target_link_libraries(TestProducerModule uipf-module)

add_library(TestConsumerModule SHARED ConsumerModule.cpp)
qt5_use_modules(TestConsumerModule Core ) #QtCore is needed for <QPlugin>
target_link_libraries(TestConsumerModule uipf-module)
