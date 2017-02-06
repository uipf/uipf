#include <QApplication>
#include <iostream>

#include "MainWindow.hpp"

using namespace std;
using namespace uipf;

int main(int argc, char *argv[])
{
	// create Qt Application
	QApplication a(argc, argv);

	// show the main window
	MainWindow w;
	w.show();

	// it is possible to give a config file as first argument
	if (argc > 1) {
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help")) {
			cout << "Usage:" << endl;
			cout << "  " << argv[0] << "             launch GUI with empty processing chain." << endl;
			cout << "  " << argv[0] << " <filename>  open processing chain from file." << endl;
			cout << "  " << argv[0] << " --help      show this help message" << endl;
		}
		w.loadProcessingChain(argv[1]);
	}

	return a.exec();
}
