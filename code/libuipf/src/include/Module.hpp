#ifdef LIBUIPF_MODULE_HPP
#error "There can only be one module per library file."
#endif
#define LIBUIPF_MODULE_HPP


#ifndef UIPF_MODULE_ID
#error "UIPF_MODULE_ID is not defined!"
#endif

#ifndef UIPF_MODULE_CLASS
#error "UIPF_MODULE_CLASS is not defined!"
#endif

#include "ModuleInterface.hpp"


class UIPF_MODULE_CLASS : public uipf::ModuleInterface {
public:
	std::string getId() const override { return UIPF_MODULE_ID; };
#ifdef UIPF_MODULE_NAME
	std::string getName() const override { return UIPF_MODULE_NAME; };
#else

	std::string getName() const override { return #UIPF_MODULE_CLASS; };
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

//#define UIPF_MODULE_INPUTS(...) //uipf::DataDescriptionMap getInputs() const override { return { __VA_ARGS__ }; }
//#define UIPF_MODULE_OUTPUTS(...) //uipf::DataDescriptionMap getOutputs() const override { return { __VA_ARGS__ }; }
//#define UIPF_MODULE_PARAMS(...) //uipf::ParamDescriptionMap getParams() const override { return { __VA_ARGS__ }; }

};

extern "C" const char* uipf_module_id(void) { return UIPF_MODULE_ID; };
extern "C" uipf::ModuleInterface* uipf_module(void) { return new UIPF_MODULE_CLASS; }

// TODO implement an uipf_module_abi_version()
