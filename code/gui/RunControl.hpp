#ifndef UIPFRAMEWORK_RUNCONTROL_HPP
#define UIPFRAMEWORK_RUNCONTROL_HPP


#include <string>
#include <vector>
#include <map>
#include <QObject>
#include <QtCore/QStringListModel>
#include <QtCore/QItemSelection>
#include <QtGui/QStandardItemModel>

#include "RunWorkerThread.h"

// TODO usability, select item in step list if none was selected and reselect on new steps added
// stop this as soon as the user selects one himself


namespace uipf {

	class MainWindow;

	class RunControl : public QObject
	{
	Q_OBJECT

	public:
		RunControl(MainWindow*);

		~RunControl() {};

		void registerWorkerSlots(RunWorkerThread*);

	private slots:

		// worker data updates
		void on_workerStepActive(std::string);
		void on_workerDataUpdated(std::string, std::string);
		void on_workerDataDeleted(std::string, std::string);

		// selection of step in run step list (via clicking)
		void on_stepSelectionChanged(const QItemSelection&);

	private:

		MainWindow* mainWindow_;

		std::string selectedStep_;
		// steps that have run and have data available
		std::vector<std::string> steps_;
		// available output data per step
		std::map<std::string, std::vector<std::string> > stepOutputs_;


		// Qt Models

		// model for the listView of processing steps for run
		QStringListModel *modelRunSteps_;

		// model for the listView of processing steps outputs
		QStandardItemModel *modelStepOutputs_;

	};
};
#endif //UIPFRAMEWORK_RUNCONTROL_HPP
