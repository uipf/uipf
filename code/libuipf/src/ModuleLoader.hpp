#ifndef UIPFRAMEWORK_MODULELOADER_HPP
#define UIPFRAMEWORK_MODULELOADER_HPP

#include <string>
#include <vector>

#include "ModuleInterface.hpp"
#include "ModuleMetaData.hpp"

namespace uipf {

	/**
	 * This class implements a Pluginloader/Moduleloader that loads modules from dynamic libraries at runtime
	 *
	 * It is based on
	 *
	 * libglibmm-2.4-dev
	 *
	 *
	 * Resources:
	 * - https://developer.gnome.org/glib/stable/glib-Dynamic-Loading-of-Modules.html
	 * - https://developer.gnome.org/glibmm/stable/classGlib_1_1Module.html#details
	 * - http://stackoverflow.com/questions/4803926/boost-plugin-choices
	 * - http://www.cplusplus.com/articles/48TbqMoL/
	 * - http://sourcey.com/building-a-simple-cpp-cross-platform-plugin-system/
	 *
	 */
	class ModuleLoader {

	public:
		// constructor
		ModuleLoader() { loaded_ = false; };

		// destructor
		~ModuleLoader(void) { reset(); };

		/**
		 * Reset the module loader.
		 *
		 * Unloads all loaded modules and clears the search paths.
		 */
		void reset();

		/**
		 * Add a search path to search for module library files.
		 * @param p the relative or absolute path of a directory to search for
		 * module libraries. This may also be a file name pointing to a module
		 * library file directly.
		 */
		void addSearchPath(const std::string& p);
		void addSearchPathsFromConfig(const std::string& configFile);


		/**
		 * @return a list of all loaded modules' names.
		 */
		std::vector<std::string> getModuleIds();

		/**
		 * @return a list of all loaded modules' names.
		 */
		std::map<std::string, std::string> getModuleNames();

		/**
		 * @return a map of all loaded modules' categories to a list of module names.
		 */
		std::map< std::string, std::vector<std::string> > getModuleCategories();

		/**
		 * @param name the id of a module to look for.
		 * @return a value indicating whether the named module exists.
		 */
		bool hasModule(const std::string& id);

		/**
		 * @param name the name of a module to look for.
		 * @return meta data information for a named module.
		 */
		ModuleMetaData getModuleMetaData(const std::string& id);

		ModuleInterface* getModuleInstance(const std::string& id);

	private:

		struct Plugin {
			std::string id;
			std::string name;
			std::string category;
			std::string description;
			void* module;
			void* instance_f;
		};
		// map: module name -> plugin loader instance which can instantiate a module
		std::map<std::string, Plugin> plugins_;


		void load();
		void loadFromPath(const std::string& path);
		void loadLibrary(const std::string& file);

		std::vector<std::string> searchPaths_;


		bool loaded_ = false;
	};
};


#endif //UIPFRAMEWORK_MODULELOADER_HPP
