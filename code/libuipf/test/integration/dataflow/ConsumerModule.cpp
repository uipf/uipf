#include <iostream>
#include "Module.hpp"

using namespace uipf;

UIPF_MODULE_BEGIN(ConsumerModule, "de.tu-berlin.cvrs.uipftest.ConsumerModule")

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

	void run() {

		std::cout << "Hello World" << std::endl;

	}

UIPF_MODULE_END

