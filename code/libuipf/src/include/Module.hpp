#ifdef LIBUIPF_MODULE_HPP
#error "There can only be one module per library file."
#endif
#define LIBUIPF_MODULE_HPP

// TODO make Ids shorter, reserve a prefix for native uipf
#ifndef UIPF_MODULE_ID
#error "UIPF_MODULE_ID is not defined!"
#endif

#ifndef UIPF_MODULE_CLASS
#error "UIPF_MODULE_CLASS is not defined!"
#endif

#include <string>
#include <map>
#include "ModuleInterface.hpp"


class UIPF_MODULE_CLASS : public uipf::ModuleInterface {
public:
	std::string getId() const override { return UIPF_MODULE_ID; };

#ifdef UIPF_MODULE_NAME
	std::string getName() const override { return UIPF_MODULE_NAME; };
#else
	// convert macro value to string
	// http://stackoverflow.com/questions/195975/how-to-make-a-char-string-from-a-c-macros-value#196018
	#define _UIPF_STR_VALUE1(arg) #arg
	#define _UIPF_STR_VALUE2(arg) _UIPF_STR_VALUE1(arg)
	std::string getName() const override { return _UIPF_STR_VALUE2(UIPF_MODULE_CLASS); };
#endif

#ifdef UIPF_MODULE_CATEGORY
	std::string getCategory() const override { return UIPF_MODULE_CATEGORY; };
#else
	std::string getCategory() const override { return "other"; };
#endif

#ifdef UIPF_MODULE_DESCRIPTION
	std::string getDescription() const override { return UIPF_MODULE_DESCRIPTION; };
#else
	std::string getDescription() const override { return ""; };
#endif

	void run() override;

#ifdef UIPF_MODULE_INPUTS
	uipf::DataDescriptionMap getInputs() const override { return { UIPF_MODULE_INPUTS }; };
#endif
#ifdef UIPF_MODULE_OUTPUTS
	uipf::DataDescriptionMap getOutputs() const override { return { UIPF_MODULE_OUTPUTS }; };
#endif
#ifdef UIPF_MODULE_PARAMS
	uipf::ParamDescriptionMap getParams() const override { return { UIPF_MODULE_PARAMS }; };
#endif

};

extern "C" const char* uipf_module_id(void) { return UIPF_MODULE_ID; };
extern "C" uipf::ModuleInterface* uipf_module(void) { return new UIPF_MODULE_CLASS; }

// TODO implement an uipf_module_abi_version()
