#include <iostream>

#define UIPF_MODULE_ID "de.tu-berlin.cvrs.uipftest.ProducerModule"
#define UIPF_MODULE_NAME "producer"
#define UIPF_MODULE_CATEGORY "test"
#define UIPF_MODULE_CLASS ProducerModule

#include "Module.hpp"

void ProducerModule::run() {

	std::cout << "Hello World ProducerModule" << std::endl;

}




/*#include <iostream>
#include "Module.hpp"

using namespace uipf;

UIPF_MODULE_BEGIN(ProducerModule, "de.tu-berlin.cvrs.uipftest.ProducerModule")

	UIPF_MODULE_NAME("producer")
	UIPF_MODULE_CATEGORY("test")
	UIPF_MODULE_DESCRIPTION("This is a test module.")

	UIPF_MODULE_OUTPUTS(
		{"string", uipf::DataDescription(uipf::String::id(), "a list of strings.", true)},
		{"int", uipf::DataDescription(uipf::String::id(), "a list of strings.", true)}
	)
	UIPF_MODULE_PARAMS(
		{"string", uipf::ParamDescription("a string.", true)},
		{"int", uipf::ParamDescription("an int.", true)}
	)

	void run() {

		std::cout << "Hello World" << std::endl;

	}

UIPF_MODULE_END

*/