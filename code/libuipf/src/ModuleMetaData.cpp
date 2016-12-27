#include "ModuleMetaData.hpp"
#include "exceptions.hpp"

using namespace uipf;
using namespace std;

ModuleMetaData::ModuleMetaData(map<string, DataDescription> inputs, map<string, DataDescription> outputs, map<string, ParamDescription> params)
{
	inputs_ = inputs;
	outputs_ = outputs;
	params_ = params;
//	description_ = description;
//	category_ = category;
}

string ModuleMetaData::getCategory() const {
	return category_;
}

string ModuleMetaData::getId() const {
	return id_;
}

string ModuleMetaData::getName() const {
	return name_;
}

string ModuleMetaData::getDescription() const {
	return description_;
}

map<string, DataDescription> ModuleMetaData::getInputs() const {
	return inputs_;
}

map<string, DataDescription> ModuleMetaData::getOutputs() const {
	return outputs_;
}

map<string, ParamDescription> ModuleMetaData::getParams() const {
	return params_;
}


// get input by name
DataDescription ModuleMetaData::getInput(const std::string& strName) const
{
	auto res = inputs_.find(strName);
	if (res != inputs_.end())
		return res->second;

	throw InvalidConfigException(std::string("input data '") + strName + std::string("' not found in Metadata!"));
}

// get output by name
DataDescription ModuleMetaData::getOutput(const std::string& strName) const
{
	auto res = outputs_.find(strName);
	if (res != outputs_.end())
		return res->second;

	throw InvalidConfigException(std::string("output data '") + strName + std::string("' not found in Metadata!"));
}

// get param by name
ParamDescription ModuleMetaData::getParam(const std::string& strName) const
{
	auto res = params_.find(strName);
	if (res != params_.end())
		return res->second;

	throw InvalidConfigException(std::string("param '") + strName + std::string("' not found in Metadata!"));
}
