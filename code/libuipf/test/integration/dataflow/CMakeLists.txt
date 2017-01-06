
add_library(TestProducerModule SHARED ProducerModule.cpp)
target_link_libraries(TestProducerModule libuipf-module)

add_library(TestConsumerModule SHARED ConsumerModule.cpp)
target_link_libraries(TestConsumerModule libuipf-module)

configure_file(test.yml test.yml)