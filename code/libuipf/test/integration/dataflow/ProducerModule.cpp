#include <iostream>
#include "data/std.hpp"

#define UIPF_MODULE_ID "de.tu-berlin.cvrs.uipftest.ProducerModule"
#define UIPF_MODULE_NAME "producer"
#define UIPF_MODULE_CATEGORY "test"
#define UIPF_MODULE_CLASS ProducerModule

#define UIPF_MODULE_OUTPUTS \
		{"string", uipf::DataDescription(uipf::data::String::id(), "a list of strings.", true)}, \
		{"int", uipf::DataDescription(uipf::data::String::id(), "a list of strings.", true)}

#define UIPF_MODULE_PARAMS \
		{"string", uipf::ParamDescription("a string.", true)}, \
		{"int", uipf::ParamDescription("an int.", true)}

#include "Module.hpp"

void ProducerModule::run() {

	std::cout << "Hello World ProducerModule" << std::endl;

}
