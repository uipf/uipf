#ifndef LIBUIPF_MODULEINTERFACE_HPP
#define LIBUIPF_MODULEINTERFACE_HPP

#include <algorithm>
#include <string>
#include <sstream>

#include "data.hpp"
#include "ModuleMetaData.hpp"
#include "exceptions.hpp"

namespace uipf {

class ModuleInterface {
	public:
		ModuleInterface(void) {};

		// destructor
		virtual ~ModuleInterface(void) {};

		// method has to be implemented in the Module
		// Module provides access to all resources described in the module meta description (params,input,output)
		virtual void run() = 0;

		// the name of the module, which can be referenced in the yaml configuration
		virtual std::string getId() const = 0;
		virtual std::string getName() const = 0;
		virtual std::string getCategory() const = 0;
		virtual std::string getDescription() const { return ""; };

		// information about execution context

		/**
		 * @return the name of the processing step this module is used in.
		 * This can be used to provide defaul values for parameters, e.g. file names.
		 */
		std::string getProcessingStepName() const {
			return pStepName_;
		}


		// data management
		virtual DataDescriptionMap getInputs() const {
			return {};
		};
		virtual DataDescriptionMap getOutputs() const {
			return {};
		};
		virtual ParamDescriptionMap getParams() const {
			return {};
		};

//		// context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
//		virtual void setContext(Context *) = 0;
//
//		// context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
//		virtual void setContext(Context*) Q_DECL_OVERRIDE;
//
//		// the name of the module, which can be referenced in the yaml configuration
//		virtual std::string name() const Q_DECL_OVERRIDE;


		// check whether named input data is given
		bool hasInputData( const std::string& strName) const {
			return input_.find(strName) != input_.end();
		};

		// returns a typesafe readonly smartpointer to input data by name if it is available
		template <typename T>
		const typename T::ptr getInputData( const std::string& strName) const;

//		// returns a typesafe smartpointer to output data by name if it is available
//		template <typename T>
//		typename T::ptr getOutputData( const std::string& strName) const;

		// sets a typesafe smartpointer for output data by name
		template <typename T>
		void setOutputData( const std::string& strName, T*);
		template <typename T>
		void setOutputData( const std::string& strName, typename T::ptr);

		// returns a typesafe parameter by name if it is available. otherwise a defaultValue is used.
		template <typename T>
		T getParam(const std::string& strName, T defaultValue) const;

	private:

		std::map < std::string, Data::ptr > input_; // TODO const!
		std::map < std::string, Data::ptr > output_;
		std::map < std::string, std::string > params_;

		std::string pStepName_;

		// Make the Runner class a friend of Module, to allow populating data
		friend class Runner;
	};

	/*
	 * templates need to be implemented in headers
	 */

	template <typename T>
	const typename T::ptr ModuleInterface::getInputData( const std::string& strName) const
	{
		auto it = input_.find(strName);
		if (it != input_.end())
		{
			//do downcasting..
			Data::ptr ptr = it->second;
			return std::dynamic_pointer_cast<T>(ptr);
		}
		else
		{
			throw InvalidConfigException(std::string("input data '") + strName + std::string("' not found!"));
		}
	}


//	template <typename T>
//	typename T::ptr ModuleInterface::getOutputData( const std::string& strName) const
//	{
//		auto it = output_.find(strName);
//		if (it != output_.end())
//		{
//			//do downcasting..
//			Data::ptr& ptr = it->second;
//			return std::dynamic_pointer_cast<T>(ptr);
//		}
//		else
//		{
//			throw InvalidConfigException(std::string("output data '") + strName + std::string("' not found!"));
//		}
//	}

	template <typename T>
	void ModuleInterface::setOutputData( const std::string& strName, T* outputData)
	{
		if (outputData != nullptr) {
			output_.insert (std::pair < std::string, Data::ptr >(strName, typename T::ptr(outputData)));
		} else {
			throw ErrorException("Can not use an uninitialized pointer as output data.");
		}
	}

	template <typename T>
	void ModuleInterface::setOutputData( const std::string& strName, typename T::ptr outputData)
	{
		if (outputData != nullptr) {
			output_.insert (std::pair < std::string, Data::ptr >(strName, outputData));
		} else {
			throw ErrorException("Can not use an uninitialized pointer as output data.");
		}
	}


	template <typename T>
	T ModuleInterface::getParam( const std::string& strName, T defaultValue) const
	{
		auto it = params_.find(strName);
		if (it != params_.end()) {
			return static_cast<T>(it->second);
		} else {
			return defaultValue;
		}
	}

	template <>
	inline int ModuleInterface::getParam( const std::string& strName, int defaultValue) const
	{
		auto it = params_.find(strName);
		if (it != params_.end() && !it->second.empty()) {
			return std::stoi(it->second);
		} else {
			return defaultValue;
		}
	}

	template <>
	inline float ModuleInterface::getParam(const std::string& strName, float defaultValue) const
	{
		auto it = params_.find(strName);
		if (it != params_.end() && !it->second.empty()) {
			// std::stof() is locale aware, meaning params are not portable between platforms
			// the following is a locale independend stof():
			float value = defaultValue;
			std::istringstream istr(it->second);
			istr.imbue(std::locale("C"));
			istr >> value;
			return value;
		} else {
			return defaultValue;
		}
	}

	template <>
	inline double ModuleInterface::getParam( const std::string& strName, double defaultValue) const
	{
		auto it = params_.find(strName);
		if (it != params_.end() && !it->second.empty()) {
			// std::stod() is locale aware, meaning params are not portable between platforms
			// the following is a locale independend stod():
			double value = defaultValue;
			std::istringstream istr(it->second);
			istr.imbue(std::locale("C"));
			istr >> value;
			return value;
		} else {
			return defaultValue;
		}
	}

	template <>
	inline bool ModuleInterface::getParam( const std::string& strName, bool defaultValue) const
	{
		auto it = params_.find(strName);
		if (it != params_.end() && !it->second.empty()) {
			std::string lower = it->second;
			std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
			return lower.compare("true") == 0
			       || lower.compare("yes") == 0
			       || lower.compare("y") == 0
			       || lower.compare("t") == 0
			       || lower.compare("1") == 0;
		} else {
			return defaultValue;
		}
	}


} //namespace


#endif //LIBUIPF_MODULEINTERFACE_HPP
