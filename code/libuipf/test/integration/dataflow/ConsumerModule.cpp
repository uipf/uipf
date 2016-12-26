#include <iostream>

#define UIPF_MODULE_ID "de.tu-berlin.cvrs.uipftest.ConsumerModule"
#define UIPF_MODULE_NAME "consumer"
#define UIPF_MODULE_CATEGORY "test"
#define UIPF_MODULE_CLASS ConsumerModule

#include "Module.hpp"

void ConsumerModule::run() {

	std::cout << "Hello World ConsumerModule" << std::endl;

}

/*


UIPF_MODULE_BEGIN(ConsumerModule, )

	UIPF_MODULE_NAME("consumer")
	UIPF_MODULE_CATEGORY("test")
	UIPF_MODULE_DESCRIPTION("This is a test module.")

	UIPF_MODULE_INPUTS(
			{"string", uipf::DataDescription(uipf::String::id(), "a list of strings.", true)},
			{"int", uipf::DataDescription(uipf::String::id(), "a list of strings.", true)}
	)
	UIPF_MODULE_PARAMS(
			{"string", uipf::ParamDescription("a string.", true)},
			{"int", uipf::ParamDescription("an int.", true)}
	)


UIPF_MODULE_END

*/