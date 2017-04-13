#include <iostream>

#define UIPF_MODULE_ID "de.tu-berlin.cvrs.uipftest.GccModule"
#define UIPF_MODULE_CATEGORY "test"
#define UIPF_MODULE_CLASS GccModule
#define UIPF_MODULE_DESCRIPTION "This module has been compiled with a simple gcc command."

#include "Module.hpp"

void GccModule::run() {

	std::cout << "Hello World GccModule" << std::endl;

}
