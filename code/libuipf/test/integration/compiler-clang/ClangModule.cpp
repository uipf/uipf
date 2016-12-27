#include <iostream>

#define UIPF_MODULE_ID "de.tu-berlin.cvrs.uipftest.ClangModule"
#define UIPF_MODULE_CATEGORY "test"
#define UIPF_MODULE_CLASS ClangModule
#define UIPF_MODULE_DESCRIPTION "This module has been compiled with a simple gcc command."

#include "Module.hpp"

void ClangModule::run() {

	std::cout << "Hello World ClangModule" << std::endl;

}
