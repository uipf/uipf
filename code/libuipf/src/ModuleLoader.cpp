#include "ModuleLoader.hpp"
#include "util.hpp"
#include <QPluginLoader>
#include <QtCore/QDir>
#include <logging.hpp>


void uipf::ModuleLoader::resetSearchPaths() {
	searchPaths_.clear();
}

void uipf::ModuleLoader::addSearchPath(std::string p) {
	searchPaths_.push_back(p);
}

std::vector<std::string> uipf::ModuleLoader::getModuleNames() const {
	using namespace std;
	vector<string> names;

	// TODO foreach metadata extract name

	return names;
}

bool uipf::ModuleLoader::hasModule(const std::string &name) const {

	// TODO foreach metadata extract name

	return false;
}

uipf::ModuleMetaData uipf::ModuleLoader::getModuleMetaData(const std::string &name) {

	// TODO foreach metadata extract name

	return uipf::ModuleMetaData();
}

uipf::Module *uipf::ModuleLoader::getModuleInstance(const std::string &name) const {
	return nullptr;
}

void uipf::ModuleLoader::load() {
	using namespace std;

	foreach(path, searchPaths_) {

		QDir pluginsDir = QDir(QString(path->c_str()));

		foreach (fileName, pluginsDir.entryList(QDir::Files))
		{
			if (!fileName->endsWith(".so")) {
				continue;
			}
			QPluginLoader* loader = new QPluginLoader(pluginsDir.absoluteFilePath(*fileName));
			QObject *plugin = loader->instance();
			if (plugin) {
				UIPF_LOG_DEBUG("found module: " << fileName->toStdString());
				Module* iModule = qobject_cast<Module* >(plugin);
				plugins_.insert( std::pair<std::string, QPluginLoader*>(iModule->getId(), loader) );
				delete iModule;
			} else {
				UIPF_LOG_ERROR(loader->errorString().toStdString());
			}
		}
	}

}
