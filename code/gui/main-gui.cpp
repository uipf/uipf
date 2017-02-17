#include <QApplication>
#include <iostream>

#include "ModuleInterface.hpp"
#include "MainWindow.hpp"
#include "logging.hpp"

using namespace std;
using namespace uipf;

class GuiConsoleLogger : public log::Logger::LogCallback {
public:
	void log(log::Logger::LogLevel lvl, const string& msg) {

		switch (lvl) {
			case log::Logger::LogLevel::ERROR:
				cout << "\033[1;31mError: \033[0m";
				break;
			case log::Logger::LogLevel::WARNING:
				cout << "\033[1;33mWarning: \033[0m";
				break;
			case log::Logger::LogLevel::INFO:
				cout << "\033[1mInfo: \033[0m";
				break;
			case log::Logger::LogLevel::DEBUG:
				cout << "\033[1;36mDebug: \033[0m";
				break;
			case log::Logger::LogLevel::TRACE:
				cout << "Trace ";
				break;
			default:
				cout << "Log: ";
		}
		cout << msg << endl;
	}

};

int main(int argc, char *argv[])
{
	// create Qt Application
	QApplication a(argc, argv);

#ifndef NDEBUG
	UIPF_LOG_LEVEL = log::Logger::LogLevel::TRACE;
	UIPF_REGISTER_LOGGER(new GuiConsoleLogger())
#else
	UIPF_LOG_LEVEL = log::Logger::LogLevel::DEBUG;
#endif

	ModuleLoader ml;
	// add default search paths for modules
	// add user configured module search paths
	#include "paths.cpp"

	// handle command line args
	if (argc > 1) {
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
			cout << "Usage:" << endl;
			cout << "  " << argv[0] << "             launch GUI with empty processing chain." << endl;
			cout << "  " << argv[0] << " [-l|--list] list available modules and exit." << endl;
			cout << "  " << argv[0] << " <filename>  open processing chain from file." << endl;
			cout << "  " << argv[0] << " [-h|--help] show this help message" << endl;
			return 0;
		}
		if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--list") == 0) {

			UIPF_LOG_LEVEL = log::Logger::LogLevel::WARNING;
			UIPF_REGISTER_LOGGER(new GuiConsoleLogger());

			std::vector<std::string> modules = ml.getModuleIds();
			for (auto mit = modules.begin(); mit != modules.end(); ++mit) {
				cout << *mit << endl;
			}
			return ml.hasErrors() ? 1 : 0;
		}
	}

	// show the main window
	MainWindow w(ml);
	w.show();

	// it is possible to give a config file as first argument
	if (argc > 1) {
		w.loadProcessingChain(argv[1]);
	}

	return a.exec();
}
