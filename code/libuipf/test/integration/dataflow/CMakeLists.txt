
add_library(TestProducerModule SHARED ProducerModule.cpp)
target_link_libraries(TestProducerModule uipf-module)

add_library(TestConsumerModule SHARED ConsumerModule.cpp)
target_link_libraries(TestConsumerModule uipf-module)

configure_file(test.yml test.yml)