#include <iostream>
#include "data/std.hpp"

#define UIPF_MODULE_ID "de.tu-berlin.cvrs.uipftest.ConsumerModule"
#define UIPF_MODULE_NAME "consumer"
#define UIPF_MODULE_CATEGORY "test"
#define UIPF_MODULE_CLASS ConsumerModule

#define UIPF_MODULE_INPUTS \
		{"string", uipf::DataDescription(uipf::data::String::id(), "a list of strings.", true)}, \
		{"int", uipf::DataDescription(uipf::data::String::id(), "a list of strings.", true)}

#define UIPF_MODULE_PARAMS \
		{"string", uipf::ParamDescription("a string.", true)}, \
		{"int", uipf::ParamDescription("an int.", true)}

#include "Module.hpp"

void ConsumerModule::run() {

	std::cout << "Hello World ConsumerModule" << std::endl;

}
