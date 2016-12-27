#ifndef LIBUIPF_MODULEINTERFACE_HPP
#define LIBUIPF_MODULEINTERFACE_HPP

#include "data.hpp"
#include "ModuleMetaData.hpp"

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

		// meta data that contains description of modules inputs, outputs and parameters
//		ModuleMetaData getMetaData() const {
//			ModuleMetaData m(
//				getInputs(),
//				getOutputs(),
//				getParams()
//			);
//		};
//		virtual DataDescriptionMap getInputs() const {
//			return {};
//		};
//		virtual DataDescriptionMap getOutputs() const {
//			return {};
//		};
//		virtual ParamDescriptionMap getParams() const {
//			return {};
//		};

//		// context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
//		virtual void setContext(Context *) = 0;
//
//		// context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
//		virtual void setContext(Context*) Q_DECL_OVERRIDE;
//
//		// the name of the module, which can be referenced in the yaml configuration
//		virtual std::string name() const Q_DECL_OVERRIDE;


		// check whether named input data is given
/*		virtual bool hasInputData( const std::string& strName) const = 0;

		// returns a typesafe readonly smartpointer to input data by name if it is available
		template <typename T>
		const typename T::ptr getInputData( const std::string& strName) const;

		// returns a typesafe smartpointer to output data by name if it is available
		template <typename T>
		typename T::ptr getOutputData( const std::string& strName) const;

		// sets a typesafe smartpointer for output data by name
		template <typename T>
		void setOutputData( const std::string& strName, T*);


		// returns a typesafe parameter by name if it is available. otherwise a defaultValue is used.
		template <typename T>
		T getParam(const std::string& strName, T defaultValue) const;

*/
};

} //namespace

// this is used for dynamic loading of Qt Plugins
// it allows modules to be loaded dynamically from shared library files
//#define UIPF_MODULE_ID "de.tu-berlin.uipf.Module"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//templates need to be implemented in headers

/*
namespace uipf {

	template <typename T>
	const typename T::ptr Module::getInputData( const std::string& strName) const
	{
		auto it = input_.find(strName);
		if (it != input_.end())
		{
			//do downcasting..
			Data::ptr& ptr = it->second;
			return std::dynamic_pointer_cast<T>(ptr);
		}
		else
		{
			throw InvalidConfigException(std::string("input data '") + strName + std::string("' not found!"));
		}
	}


	template <typename T>
	typename T::ptr Module::getOutputData( const std::string& strName) const
	{
		auto it = output_.find(strName);
		if (it != output_.end())
		{
			//do downcasting..
			Data::ptr& ptr = it->second;
			return std::dynamic_pointer_cast<T>(ptr);
		}
		else
		{
			throw InvalidConfigException(std::string("output data '") + strName + std::string("' not found!"));
		}
	}

	template <typename T>
	void Module::setOutputData( const std::string& strName, T* outputData)
	{
		output_.insert (std::pair < std::string, Data::ptr >(strName, typename T::ptr(outputData)));
	}


	template <typename T>
	T Module::getParam( const std::string& strName, T defaultValue) const
	{
		if (params_.find(strName) != params_.end())
		{
			return static_cast<T>(params_[strName]);
		}
		else
		{
			return defaultValue;
		}
	}

	template <>
	inline int Module::getParam( const std::string& strName, int defaultValue) const
	{
		if (params_.find(strName) != params_.end() && !params_[strName].empty())
		{
			return std::stoi(params_[strName]);
		}
		else
		{
			return defaultValue;
		}
	}

	template <>
	inline float Module::getParam(const std::string& strName, float defaultValue) const
	{
		if (params_.find(strName) != params_.end() && !params_[strName].empty())
		{
			// std::stof() is locale aware, meaning params are not portable between platforms
			// the following is a locale independend stof():
			float value = defaultValue;
			std::istringstream istr(params_[strName]);
			istr.imbue(std::locale("C"));
			istr >> value;
			return value;
		}
		else
		{
			return defaultValue;
		}
	}

	template <>
	inline double Module::getParam( const std::string& strName, double defaultValue) const
	{
		if (params_.find(strName) != params_.end() && !params_[strName].empty())
		{
			// std::stod() is locale aware, meaning params are not portable between platforms
			// the following is a locale independend stod():
			double value = defaultValue;
			std::istringstream istr(params_[strName]);
			istr.imbue(std::locale("C"));
			istr >> value;
			return value;
		}
		else
		{
			return defaultValue;
		}
	}

	template <>
	inline bool Module::getParam( const std::string& strName, bool defaultValue) const
	{
		if (params_.find(strName) != params_.end() && !params_[strName].empty())
		{
			std::string lower = utils::toLower(params_[strName]);
			return lower.compare("true") == 0
			       || lower.compare("yes") == 0
			       || lower.compare("y") == 0
			       || lower.compare("t") == 0
			       || lower.compare("1") == 0;
		}
		else
		{
			return defaultValue;
		}
	}


} //namespace
*/



#endif //LIBUIPF_MODULEINTERFACE_HPP
