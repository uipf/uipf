#include "ModuleLoader.hpp"
#include <QPluginLoader>
#include <QtCore/QDir>
#include <logging.hpp>


void uipf::ModuleLoader::reset() {
	UIPF_LOG_TRACE("Reset module loader.");
	searchPaths_.clear();
	// delete all the pluginloaders
	for (auto it = plugins_.begin(); it!=plugins_.end(); ++it) {
		delete it->second;
	}
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

	// TODO foreach metadata extract name

	return names;
}

bool uipf::ModuleLoader::hasModule(const std::string &name) {
	using namespace std;

	if (!loaded_) load();

	// TODO foreach metadata extract name

	return false;
}

uipf::ModuleMetaData uipf::ModuleLoader::getModuleMetaData(const std::string &name) {
	using namespace std;

	if (!loaded_) load();

	// TODO foreach metadata extract name

	return uipf::ModuleMetaData();
}

uipf::Module *uipf::ModuleLoader::getModuleInstance(const std::string &name) {
	using namespace std;

	if (!loaded_) load();


	UIPF_LOG_TRACE("Instantiate module: ", name);
	return nullptr;
}

void uipf::ModuleLoader::load() {
	using namespace std;

	UIPF_LOG_TRACE("Loading modules...");

	foreach(string path, searchPaths_)
	{
		QDir pluginsDir = QDir(QString(path.c_str()));

		foreach (QString fileName, pluginsDir.entryList(QDir::Files))
		{
			if (!fileName.endsWith(".so")) {
				continue;
			}
			UIPF_LOG_TRACE("Trying file: ", fileName.toStdString());
			QPluginLoader* loader = new QPluginLoader(pluginsDir.absoluteFilePath(fileName));
			QObject *plugin = loader->instance();
			if (plugin) {
				UIPF_LOG_DEBUG("found module: ", fileName.toStdString());
				Module* iModule = qobject_cast<Module* >(plugin);
				plugins_.insert( std::pair<std::string, QPluginLoader*>(iModule->getId(), loader) );
				delete iModule;
			} else {
				UIPF_LOG_ERROR(loader->errorString().toStdString());
			}
		}
	}
	loaded_ = true;
	UIPF_LOG_DEBUG("Loaded ", plugins_.size(), " modules.");
}
