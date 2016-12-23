#ifndef UIPFRAMEWORK_MODULELOADER_HPP
#define UIPFRAMEWORK_MODULELOADER_HPP

#include <string>
#include <vector>
#include <QtCore/QPluginLoader>

#include "Module.hpp"
#include "ModuleMetaData.hpp"

namespace uipf {

	class ModuleLoader {

	public:
		// constructor
		ModuleLoader() { loaded_ = false; };

		// destructor
		~ModuleLoader(void) {};

		void reset();
		void addSearchPath(std::string p);

		// returns a list of all loaded modules names
		std::vector<std::string> getModuleNames();

		// returns a value indicating whether the named module exists
		bool hasModule(const std::string& name);

		// returns meta data information for a named module
		ModuleMetaData getModuleMetaData(const std::string& name);

		Module* getModuleInstance(const std::string& name);

	private:

		void load();

		std::vector<std::string> searchPaths_;

		// map: module name -> plugin loader instance which can instantiate a module
		std::map<std::string, QPluginLoader*> plugins_;

		bool loaded_ = false;
	};
};


#endif //UIPFRAMEWORK_MODULELOADER_HPP
