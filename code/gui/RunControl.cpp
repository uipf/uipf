#include "ModuleInterface.hpp"
#include "RunControl.hpp"
#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include "data/list.hpp"

#include <QTimer>
#include <QWindow>

#include <algorithm>

using namespace uipf;
using namespace std;

RunControl::RunControl(uipf::MainWindow *mw) : mainWindow_(mw), workerThread_(nullptr)
{
	// create model for step list in run mode
	modelRunSteps_ = new QStringListModel(this);
	mw->ui->listRunSteps->setModel(modelRunSteps_);
	// step list is not editable
	mw->ui->listRunSteps->setEditTriggers(QAbstractItemView::NoEditTriggers);
	// react to selection of the entries
	connect(mw->ui->listRunSteps->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
	        this, SLOT(on_stepSelectionChanged(QItemSelection)));


	modelStepOutputs_ = new QStandardItemModel(this);
	modelStepOutputs_->setColumnCount(4);
	QStandardItem* item0 = new QStandardItem("Output:");
	QStandardItem* item1 = new QStandardItem("Type:");
	QStandardItem* item2 = new QStandardItem("Serialize:");
	QStandardItem* item3 = new QStandardItem("Visualize:");
	item0->setToolTip(QString("Name of the module output."));
	item1->setToolTip(QString("Data type of the module output."));
	modelStepOutputs_->setHorizontalHeaderItem(0, item0);
	modelStepOutputs_->setHorizontalHeaderItem(1, item1);
	modelStepOutputs_->setHorizontalHeaderItem(2, item2);
	modelStepOutputs_->setHorizontalHeaderItem(3, item3);

	mw->ui->tableOutputs->setModel(modelStepOutputs_);
	mw->ui->tableOutputs->setEnabled(false);
	mw->ui->tableOutputs->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	mw->ui->tableOutputs->verticalHeader()->setVisible(false);
	mw->ui->tableOutputs->setSelectionMode(QAbstractItemView::NoSelection);
	for (int c = 0; c < mw->ui->tableOutputs->horizontalHeader()->count(); ++c) {
		mw->ui->tableOutputs->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
	}

	// button events
	connect(mw->ui->buttonRun, SIGNAL(clicked()), this, SLOT(on_buttonRun()));
	connect(mw->ui->buttonStep, SIGNAL(clicked()), this, SLOT(on_buttonStep()));
	connect(mw->ui->buttonStop, SIGNAL(clicked()), this, SLOT(on_buttonStop()));
	connect(mw->ui->buttonClear, SIGNAL(clicked()), this, SLOT(on_buttonClear()));
	mw->ui->buttonStop->setEnabled(false);
	mw->ui->buttonClear->setEnabled(false);

	// view button signal mapper
	vizButtonMapper_ = new QSignalMapper(this);
	connect(vizButtonMapper_, SIGNAL(mapped(QString)), this, SLOT(on_vizButtonClick(QString)));
	listVizButtonMapper_ = new QSignalMapper(this);
	connect(listVizButtonMapper_, SIGNAL(mapped(QString)), this, SLOT(on_listVizButtonClick(QString)));
	// serialize button signal mapper
	serializeButtonMapper_ = new QSignalMapper(this);
	connect(serializeButtonMapper_, SIGNAL(mapped(QString)), this, SLOT(on_serializeButtonClick(QString)));

}

void RunControl::registerWorkerSlots(RunWorkerThread *wt)
{
	connect(wt, SIGNAL(eventStepActive(std::string, int, int)),
	        this, SLOT (on_workerStepActive(std::string, int, int)));
	connect(wt, SIGNAL(eventStepFinished(std::string, bool)),
	        this, SLOT (on_workerStepFinished(std::string, bool)));
	connect(wt, SIGNAL(eventDataUpdated(std::string, std::string, Data::ptr)),
	        this, SLOT (on_workerDataUpdated(std::string, std::string, Data::ptr)));
	connect(wt, SIGNAL(eventDataDeleted(std::string, std::string)),
	        this, SLOT (on_workerDataDeleted(std::string, std::string)));

	// Setup callback for cleanup when it finishes
	connect(wt, SIGNAL(finished()),  this, SLOT(on_workerFinished()));

}

void RunControl::on_workerStepActive(std::string stepName, int number, int count)
{
	UIPF_LOG_DEBUG("step active: ", stepName);

	steps_.push_back(stepName);
	stepOutputs_.insert(pair<string, vector<StepOutput> >(stepName, vector<StepOutput>()));

	int row = modelRunSteps_->rowCount();
	modelRunSteps_->insertRow(row);
	QModelIndex index = modelRunSteps_->index(row);
	modelRunSteps_->setData(index, QString::fromStdString(stepName));

	mainWindow_->ui->progressBar->setFormat(QString::fromStdString(string("Chain: %p% (") + to_string(number) + string("/") + to_string(count) + string(")")));
	currentStep = number;
	stepCount = count;
	// select newly inserted item TODO
//	mainWindow_->ui->listRunSteps->setCurrentIndex(index);
}

void RunControl::on_workerStepFinished(std::string stepName, bool success)
{
	std::vector<std::string> names;
	names.push_back(stepName);
	if (success) {
		mainWindow_->graphView_->selectNodesInGraphView(names, gui::GOOD, false);
	} else {
		mainWindow_->graphView_->selectNodesInGraphView(names, gui::ERROR, false);
	}

}

void RunControl::on_workerDataUpdated(std::string stepName, std::string outputName, Data::ptr data)
{
	UIPF_LOG_TRACE("on_workerDataUpdated: ", stepName, outputName);

	auto pos = stepOutputs_.find(stepName);
	if (pos != stepOutputs_.end()) {
		// insert output name to the list if it did not exist
		bool exists = false;
		for(StepOutput o: pos->second) {
			if (o.name == outputName) {
				exists = true;
				break;
			}
		}
		if (!exists) {
			StepOutput output;
			output.name = outputName;
			output.data = data;
			pos->second.push_back(output);

			// TODO if step is selected ,update UI
		}
	}
}

void RunControl::on_workerDataDeleted(std::string stepName, std::string outputName)
{
	UIPF_LOG_TRACE("on_workerDataDeleted: ", stepName, outputName);

	// TODO add name to list of deleted data

	// TODO if step is selected ,update UI
}


// gets called when a processing step is selected
void RunControl::on_stepSelectionChanged(const QItemSelection& selection){
	if(selection.indexes().isEmpty()) {
		modelStepOutputs_->setRowCount(0);
		mainWindow_->ui->tableOutputs->setEnabled(false);
	} else {
//		mainWindow_->ui->listRunSteps->setCurrentIndex(selection.indexes().first()); // TODO WTF?

		selectedStep_ = modelRunSteps_->data(selection.indexes().first(), Qt::DisplayRole).toString().toStdString();
		UIPF_LOG_TRACE("selected step: ", selectedStep_);

		// TODO move below to separate function

		// reset outputs
		modelStepOutputs_->setRowCount(0);

		auto outputData = stepOutputs_.find(selectedStep_);
		if (outputData != stepOutputs_.end()) {
			int r = 0;
			for(StepOutput output: outputData->second) {
				QStandardItem* item = new QStandardItem(output.name.c_str());
				item->setEditable(false);
				// TODO set tooltip from model data description

				QStandardItem* type = new QStandardItem(output.data->getType().c_str());
				type->setToolTip(output.data->getType().c_str());
				type->setEditable(false);

				modelStepOutputs_->setItem(r, 0, item);
				modelStepOutputs_->setItem(r, 1, type);
				if (output.data->isSerializable()) {
					QPushButton *button = new QPushButton(QString("show"));

					connect(button, SIGNAL(clicked()), serializeButtonMapper_, SLOT(map()));
					serializeButtonMapper_->setMapping(button, QString::fromStdString(output.name));
					mainWindow_->ui->tableOutputs->setIndexWidget(
							mainWindow_->ui->tableOutputs->model()->index(r, 2),
							button
					);
				} else {
					QStandardItem* noser = new QStandardItem("n/a");
					noser->setToolTip("Data type is not serializable.");
					noser->setEditable(false);
					modelStepOutputs_->setItem(r, 2, noser);
				}
				int a = r;
				if (output.data->isList()) { // TODO map
					QPushButton *button = new QPushButton("items");
					connect(button, SIGNAL(clicked()), vizButtonMapper_, SLOT(map()));
					vizButtonMapper_->setMapping(button, QString::fromStdString(to_string(output.name.size()) + " " + output.name + string("items")));
					mainWindow_->ui->tableOutputs->setIndexWidget(
							mainWindow_->ui->tableOutputs->model()->index(r++, 3),
							button
					);
				}
				if (!output.data->isList() && output.data->visualizations().size() == 0) {
					QStandardItem* noviz = new QStandardItem("n/a");
					noviz->setToolTip("No Visualization options available for this data type.");
					noviz->setEditable(false);
					modelStepOutputs_->setItem(r, 3, noviz);
				} else {
					for (string v: output.data->visualizations()) {
						if (r > a) {
							QStandardItem* dummyItem0 = new QStandardItem();
							dummyItem0->setEditable(false);
							modelStepOutputs_->setItem(r, 0, dummyItem0);
							QStandardItem* dummyItem1 = new QStandardItem();
							dummyItem1->setEditable(false);
							modelStepOutputs_->setItem(r, 1, dummyItem1);
							QStandardItem* dummyItem2 = new QStandardItem();
							dummyItem2->setEditable(false);
							modelStepOutputs_->setItem(r, 2, dummyItem2);
						}
						UIPF_LOG_TRACE("add vis option ", v);
						QPushButton *button = new QPushButton(QString::fromStdString(v));

						connect(button, SIGNAL(clicked()), vizButtonMapper_, SLOT(map()));
						vizButtonMapper_->setMapping(button, QString::fromStdString(to_string(output.name.size()) + " " + output.name + v));
						mainWindow_->ui->tableOutputs->setIndexWidget(
								mainWindow_->ui->tableOutputs->model()->index(r++, 3),
								button
						);
					}
				}
				r++;
			}
		}
		mainWindow_->ui->tableOutputs->setEnabled(true);
	}
}

void RunControl::on_vizButtonClick(QString outputName)
{
	UIPF_LOG_TRACE("Viz button: ", outputName.toStdString());

	// split string into parts: "<len(output)> <output><vizoption>"
	string outputAndViz = outputName.toStdString();
	unsigned long pos = outputAndViz.find(" ");
	long split = atol(outputAndViz.substr(0, pos).c_str());
	string output = outputAndViz.substr(pos + 1, (unsigned)split);
	string option = outputAndViz.substr(pos + 1 + split);

	UIPF_LOG_TRACE("Viz button: split: '", output, "' , '", option, "'");

	for(StepOutput o: stepOutputs_[selectedStep_]) {
		if (o.name == output) {
			vizData(o.data, option, o.name);
			return;
		}
	}
}

// TODO add support for nested lists
void RunControl::on_listVizButtonClick(QString outputName)
{
	UIPF_LOG_TRACE("List Viz button: ", outputName.toStdString());

	// split string into parts: "<len(output)> <output><vizoption>"
	string outputAndViz = outputName.toStdString();
	unsigned long pos = outputAndViz.find(" ");
	long split = atol(outputAndViz.substr(0, pos).c_str());
	string output = outputAndViz.substr(pos + 1, (unsigned)split);
	string vizoption = outputAndViz.substr(pos + 1 + split);

	// split vizoption into parts again: "<len(output)> <output><vizoption>"
	pos = vizoption.find(" ");
	split = atol(vizoption.substr(0, pos).c_str());
	unsigned long itemid = atol(vizoption.substr(pos + 1, (unsigned)split).c_str());
	string option = vizoption.substr(pos + 1 + split);

	UIPF_LOG_TRACE("List Viz button: split: '", output, "' , '", option, "'");

	for(StepOutput o: stepOutputs_[selectedStep_]) {
		if (o.name == output) {
			data::List::ptr list = static_pointer_cast<data::List>(o.data);
			vizData(list->getContent()[itemid], option, o.name);
			return;
		}
	}
}

void RunControl::vizData(Data::ptr d, std::string option, std::string outputName)
{
	if (d->isList() && option.compare("items") == 0) {

		// TODO subwindow creation should maybe better be in Mainwindow, move this there.

		// visualize list data type
		QWidget* listWindow = new QWidget();
		listWindow->setMinimumWidth(500);
		listWindow->setMinimumHeight(400);

		QTableView* listTable = new QTableView(listWindow);
		QVBoxLayout* listWindowLayout = new QVBoxLayout();
		listWindowLayout->addWidget(listTable);

		listWindow->setLayout(listWindowLayout);
		listWindow->setWindowTitle(QString("List items in output '") + QString::fromStdString(outputName) + QString("' of step '") + QString::fromStdString(selectedStep_) + QString("'"));
		listWindow->setVisible(true);

		// TODO this code is duplicate with the constructor of this class
		QStandardItemModel* tableModel = new QStandardItemModel();
		listTable->setModel(tableModel);
		tableModel->setColumnCount(3);
		QStandardItem* item0 = new QStandardItem("Item:");
		QStandardItem* item1 = new QStandardItem("Type:");
		QStandardItem* item2 = new QStandardItem("Visualize:");
		item0->setToolTip(QString("Name of the module output."));
		item1->setToolTip(QString("Data type of the module output."));
		tableModel->setHorizontalHeaderItem(0, item0);
		tableModel->setHorizontalHeaderItem(1, item1);
		tableModel->setHorizontalHeaderItem(2, item2);

		listTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
		listTable->verticalHeader()->setVisible(false);
		listTable->setSelectionMode(QAbstractItemView::NoSelection);
		for (int c = 0; c < listTable->horizontalHeader()->count(); ++c) {
			listTable->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
		}


		int r = 0;
		for(Data::ptr item: d->getListContent()) {
			// TODO code in this loop is pretty much duplicate with on_stepSelectionChanged(), refactor?

			std::string name = item->getName();
			if (name.empty()) {
				name = to_string(r);
			}
			QStandardItem* rowId = new QStandardItem(QString::fromStdString(name));
			rowId->setToolTip(name.c_str());
			rowId->setEditable(false);

			QStandardItem* type = new QStandardItem(item->getType().c_str());
			type->setToolTip(item->getType().c_str());
			type->setEditable(false);

			tableModel->setItem(r, 0, rowId);
			tableModel->setItem(r, 1, type);

			if (item->visualizations().size() == 0) {
				QStandardItem* noviz = new QStandardItem("n/a");
				noviz->setToolTip("No Visualization options available for this data type.");
				noviz->setEditable(false);
				tableModel->setItem(r, 2, noviz);
			} else {
				int a = 0;
				for (string v: item->visualizations()) {
					UIPF_LOG_TRACE("add vis option ", v);
					QPushButton *button = new QPushButton(QString::fromStdString(v));

					connect(button, SIGNAL(clicked()), listVizButtonMapper_, SLOT(map()));
					string vizoption = to_string(to_string(r).size()) + " " + to_string(r) + v;
					listVizButtonMapper_->setMapping(button, QString::fromStdString(to_string(outputName.size()) + " " + outputName + vizoption));
					if (a > 0) {
						tableModel->setItem(r, 2 + a, new QStandardItem());
					}
					listTable->setIndexWidget(
							listTable->model()->index(r, 2 + a++),
							button
					);
				}
			}
			r++;
		}

		mainWindow_->createdWindwows_.push_back(listWindow);
		mainWindow_->closeWindowsAct->setEnabled(true);


	} else {

		// visualize single data type
		VisualizationContext& context = *(mainWindow_->visualizationContext_);
		try {
			d->visualize(option, context);
		} catch (const std::exception &e) {
			UIPF_LOG_ERROR(string("Error in visualization: ") + e.what());
			return;
		} catch(...) {
			UIPF_LOG_ERROR(string("Unknown error occurred while trying to show visualization."));
			return;
		}

	}
}

void RunControl::on_serializeButtonClick(QString outputName)
{
	UIPF_LOG_TRACE("Serialize button: ", outputName.toStdString());

	for(StepOutput o: stepOutputs_[selectedStep_]) {
		if (o.name == outputName.toStdString()) {
			Data::ptr d = o.data;

			ostringstream s;
			try {
				d->serialize(s);
			} catch (const std::exception &e) {
				UIPF_LOG_ERROR(string("Error in serialisation: ") + e.what());
				return;
			} catch(...) {
				UIPF_LOG_ERROR(string("Unknown error occurred while trying to serialize data."));
				return;
			}

			// TODO this is duplicate with mainwindow, merge it

			QWidget* textWindow = new QWidget();

			QTextEdit* textEdit = new QTextEdit(textWindow);
			textEdit->setPlainText(QString::fromStdString(s.str()));
			textEdit->setReadOnly(true);
			QVBoxLayout* textWindowLayout = new QVBoxLayout();
			textWindowLayout->addWidget(textEdit);

			textWindow->setLayout(textWindowLayout);
			textWindow->setWindowTitle(QString("Serialization of output '") + outputName + QString("' from step '") + QString::fromStdString(selectedStep_) + QString("'"));
			textWindow->setVisible(true);

			mainWindow_->createdWindwows_.push_back(textWindow);
			mainWindow_->closeWindowsAct->setEnabled(true);

			return;
		}
	}
}

//this gets called from Backgroundthread when its work is finished or when it gets terminated by stop()
void RunControl::on_workerFinished()
{
	UIPF_LOG_TRACE("Worker finished.");

	bool clear = (runStatus == StatusStopping);

	mainWindow_->ui->progressBarModule->setHidden(true);
	mainWindow_->ui->progressBarModule->update();

	// upate buttons
	if (currentStep >= stepCount) {
		mainWindow_->ui->buttonRun->setEnabled(false);
		mainWindow_->ui->buttonRun->setText(tr("Run"));
		mainWindow_->ui->buttonStep->setEnabled(false);
		mainWindow_->ui->buttonStep->setText(tr("Step"));
		runStatus = StatusDone;
	} else {
		mainWindow_->ui->buttonRun->setEnabled(true);
		mainWindow_->ui->buttonRun->setText(tr("Resume"));
		mainWindow_->ui->buttonStep->setEnabled(true);
		mainWindow_->ui->buttonStep->setText(tr("Step"));
		runStatus = StatusPaused;
	}
	mainWindow_->ui->buttonStop->setEnabled(false);
	mainWindow_->ui->buttonStop->setText(tr("Stop"));
	mainWindow_->ui->buttonClear->setEnabled(true);

	if (clear) {
		on_buttonClear();
	}

}

void RunControl::on_buttonRun()
{
	UIPF_LOG_TRACE("button run");

	if (runStatus == StatusRunningChain) {
		// pausing...

		mainWindow_->ui->buttonRun->setText(tr("Pause..."));
		mainWindow_->ui->buttonRun->setEnabled(false);
		workerThread_->pause();

		// TODO handle reply from thread

	} else {
		runChain();
	}

}

void RunControl::on_buttonStep() {
	UIPF_LOG_TRACE("button step");

	runStep();
}

void RunControl::on_buttonStop() {
	UIPF_LOG_TRACE("button stop");

	if (workerThread_ == nullptr) return;

	mainWindow_->ui->buttonStop->setText(tr("Stop..."));
	mainWindow_->ui->buttonStop->setEnabled(false);
	mainWindow_->ui->buttonStop->update();
	mainWindow_->ui->buttonRun->setEnabled(false);
	mainWindow_->ui->buttonRun->update();
	mainWindow_->ui->buttonStep->setEnabled(false);
	mainWindow_->ui->buttonStep->update();
	mainWindow_->ui->buttonClear->setEnabled(false);
	mainWindow_->ui->buttonClear->update();

	mainWindow_->ui->listRunSteps->setEnabled(false);
	mainWindow_->ui->tableOutputs->setEnabled(false);
	mainWindow_->ui->progressBar->setUpdatesEnabled(false);
	mainWindow_->ui->progressBarModule->setUpdatesEnabled(false);

	runStatus = StatusStopping;

	//signal modules to stop
	UIPF_LOG_TRACE("workerThread_->stop();");
	workerThread_->stop();
	// give them some time before killing it
	QTimer::singleShot(3000, this, SLOT(killWorker()));
}

void RunControl::killWorker()
{
	if (workerThread_ == nullptr) {
		return;
	}

	//kill if not ready yet
	UIPF_LOG_TRACE("workerThread_->terminate();");
	workerThread_->terminate();
}


void RunControl::on_buttonClear() {
	UIPF_LOG_TRACE("button clear");

	mainWindow_->ui->buttonRun->setText(tr("Run"));
	mainWindow_->ui->buttonRun->setEnabled(true);
	mainWindow_->ui->buttonStep->setEnabled(true);
	mainWindow_->ui->buttonStop->setEnabled(false);
	mainWindow_->ui->buttonClear->setEnabled(false);

	mainWindow_->ui->progressBar->setValue(0);
	mainWindow_->ui->progressBar->setHidden(true);
	mainWindow_->ui->progressBar->setUpdatesEnabled(true);
	mainWindow_->ui->progressBar->update();
	mainWindow_->ui->progressBarModule->setHidden(true);
	mainWindow_->ui->progressBarModule->update();
	mainWindow_->ui->progressBarModule->setUpdatesEnabled(true);

	modelRunSteps_->removeRows(0, modelRunSteps_->rowCount());
	modelStepOutputs_->setRowCount(0);
	mainWindow_->ui->tableOutputs->setEnabled(false);
	mainWindow_->ui->listRunSteps->setEnabled(true);

	selectedStep_ = "";
	stepOutputs_.clear();
	steps_.clear();

	mainWindow_->graphView_->clearSelectionInGraphView();

	delete workerThread_;
	workerThread_ = nullptr;
}



void RunControl::runChain()
{
	if (!mainWindow_->validateChain()) {
		return;
	}

	if (workerThread_ == nullptr) {
		// first step, initialize worker
		createWorker();
	}

	// stop is now activated and run unactivated
	mainWindow_->stopAct->setEnabled(true);
	mainWindow_->runAct->setEnabled(false);
	mainWindow_->ui->progressBar->setHidden(false);
	mainWindow_->ui->progressBar->update();
	mainWindow_->ui->progressBarModule->setHidden(true);

	mainWindow_->ui->buttonRun->setEnabled(true);
	mainWindow_->ui->buttonRun->setText(tr("Pause"));
	mainWindow_->ui->buttonStep->setEnabled(false);
	mainWindow_->ui->buttonStop->setEnabled(true);
	mainWindow_->ui->buttonClear->setEnabled(false);

	runStatus = StatusRunningChain;

	workerThread_->setMode(RunWorkerThread::RUN_CHAIN);

	workerThread_->start(); // This invokes WorkerThread::run in a new thread
}

void RunControl::runStep()
{
	if (!mainWindow_->validateChain()) {
		return;
	}

	if (workerThread_ == nullptr) {
		// first step, initialize worker
		createWorker();
	}

	mainWindow_->ui->buttonRun->setEnabled(false);
	mainWindow_->ui->buttonStep->setText(tr("Step..."));
	mainWindow_->ui->buttonStep->setEnabled(false);
	mainWindow_->ui->buttonStop->setEnabled(true);
	mainWindow_->ui->buttonClear->setEnabled(false);

	// stop is now activated and run unactivated
	mainWindow_->stopAct->setEnabled(true);
	mainWindow_->runAct->setEnabled(false);
	mainWindow_->ui->progressBar->setHidden(false);
	mainWindow_->ui->progressBar->update();
	mainWindow_->ui->progressBarModule->setHidden(true);

	runStatus = StatusRunningStep;

	workerThread_->setMode(RunWorkerThread::RUN_STEP);

	workerThread_->start(); // This invokes WorkerThread::run in a new thread
}

void RunControl::createWorker()
{
	workerThread_ = new RunWorkerThread(mainWindow_->conf_, mainWindow_->mm_);

	// progress updates
	connect(workerThread_, SIGNAL(eventUpdateGlobalProgress(int, int)),
	        mainWindow_, SLOT (on_reportGlobalProgress(int, int)));
	connect(workerThread_, SIGNAL(eventUpdateModuleProgress(int, int)),
	        mainWindow_, SLOT (on_reportModuleProgress(int, int)));
	// data updates
	registerWorkerSlots(workerThread_);
}
