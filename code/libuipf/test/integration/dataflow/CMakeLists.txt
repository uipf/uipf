
add_library(TestProducerModule SHARED ProducerModule.cpp)
target_link_libraries(TestProducerModule uipf)
qt5_use_modules(TestProducerModule Core ) #QtCore is needed for <QPlugin>

add_library(TestConsumerModuleModule SHARED ConsumerModule.cpp)
target_link_libraries(TestConsumerModuleModule uipf)
qt5_use_modules(TestConsumerModuleModule Core ) #QtCore is needed for <QPlugin>

