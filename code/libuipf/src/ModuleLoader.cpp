#include "ModuleLoader.hpp"
#include "util.hpp"
#include <logging.hpp>
//#include <glib-2.0/gmodule.h>
#include <glibmm/module.h>


#include <boost/filesystem.hpp>


void uipf::ModuleLoader::reset() {
	UIPF_LOG_TRACE("Reset module loader.");
	searchPaths_.clear();
	// delete all the pluginloaders
	for (auto it = plugins_.begin(); it!=plugins_.end(); ++it) {
		Glib::Module* p = (Glib::Module*) it->second.instance;
		delete p;
	}
	plugins_.clear();
	loaded_ = false;
}

void uipf::ModuleLoader::addSearchPath(std::string p) {
	UIPF_LOG_DEBUG("Added module search dir: ", p);
	searchPaths_.push_back(p);
}

std::vector<std::string> uipf::ModuleLoader::getModuleNames() {
	using namespace std;

	if (!loaded_) load();

	vector<string> names;

	// foreach metadata extract name
	uipf_foreach(p, plugins_) {
		names.push_back(p->second.name);
	}

	return names;
}

bool uipf::ModuleLoader::hasModule(const std::string &name) {
	using namespace std;

	if (!loaded_) load();

	// foreach metadata extract name
	uipf_foreach(p, plugins_) {
		if (p->second.name == name) {
			return true;
		}
	}

	return false;
}

uipf::ModuleMetaData uipf::ModuleLoader::getModuleMetaData(const std::string &name) {
	using namespace std;

	if (!loaded_) load();

	// TODO foreach metadata extract name

	return uipf::ModuleMetaData();
}

uipf::ModuleInterface *uipf::ModuleLoader::getModuleInstance(const std::string &name) {
	using namespace std;

	if (!loaded_) load();


	UIPF_LOG_TRACE("Instantiate module: ", name);
	return nullptr;
}



void uipf::ModuleLoader::load() {
	UIPF_LOG_TRACE("Loading modules...");

	for(const std::string& sPath : searchPaths_) {
		loadFromPath(sPath);
	}
	loaded_ = true;
	UIPF_LOG_DEBUG("Loaded ", plugins_.size(), " modules.");
}

void uipf::ModuleLoader::loadFromPath(const std::string& sPath) {
	using namespace boost::filesystem;

	UIPF_LOG_TRACE("Looking for modules in ", sPath);

	std::string s(sPath);
	path p(s.c_str());
	if (p.is_relative()) {
		p = system_complete(p);
	}

	try {
		if (!exists(p)) {
			UIPF_LOG_WARNING("Module search path does not exist: ", sPath);
		} else if (is_directory(p)) {

			directory_iterator end_itr;
			for (directory_iterator itr( p ); itr != end_itr; ++itr) {
				// TODO scan different files on windows: .dll
				if (is_regular_file(itr->path()) && extension(itr->path()) == std::string(".so")) {
					loadLibrary(itr->path().string());
				}
			}

		} else if (is_regular_file(p)) {
			if (extension(p) == std::string(".so")) {
				loadLibrary(p.string());
			}
		} else {
			UIPF_LOG_WARNING("Module search path is not a regular file or directory: ", sPath);
		}
	}
	catch (const filesystem_error& ex) {
		UIPF_LOG_ERROR("Failed to read module search path: ", ex.what());
	}
}

void uipf::ModuleLoader::loadLibrary(const std::string& file) {

	UIPF_LOG_TRACE("Trying file: ", file);

	if (!Glib::Module::get_supported()) {
		UIPF_LOG_ERROR("Loading dynamic libraries is not supported on your platform.");
		return;
	}

	Glib::Module* libModule = new Glib::Module(file);
	UIPF_LOG_TRACE("Found Module: ", libModule->get_name());

	// a module defines the following functions, cmp. Module.hpp
	typedef char* uipf_module_id_f();
	typedef ModuleInterface* uipf_module_f();

	void* uipf_module_id_p = nullptr;
	void* uipf_module_p = nullptr;

	// test whether the module is valid
	if (libModule->get_symbol("uipf_module_id", uipf_module_id_p)
	 && libModule->get_symbol("uipf_module", uipf_module_p)) {

		uipf_module_id_f* fun_id = (uipf_module_id_f *) uipf_module_id_p;
		std::string moduleId(fun_id());

		uipf_module_f* fun_instance = (uipf_module_f *) uipf_module_p;
		ModuleInterface* instance = fun_instance();

		UIPF_LOG_TRACE("loaded module id: ", instance->getId());
		UIPF_LOG_TRACE("module name: ", instance->getName());
		UIPF_LOG_TRACE("module descr: ", instance->getDescription());
		UIPF_LOG_TRACE("module category: ", instance->getCategory());

		Plugin p;
		p.id = moduleId;
		p.name = instance->getName();
		p.description = instance->getDescription();
		p.category = instance->getCategory();
		p.instance = (void*) libModule;
		plugins_.insert(std::pair<std::string, Plugin>(moduleId, p));

		delete instance;
	} else {
		UIPF_LOG_ERROR("Failed to load module: ", libModule->get_last_error());
		delete libModule;
	}
}