#include "ModuleInterface.hpp"
#include "RunControl.hpp"
#include "MainWindow.hpp"
#include "ui_mainwindow.h"

#include <algorithm>

using namespace uipf;
using namespace std;

RunControl::RunControl(uipf::MainWindow *mw) : mainWindow_(mw)
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
	modelStepOutputs_->setColumnCount(3);
	QStandardItem* item0 = new QStandardItem("Output:");
	QStandardItem* item1 = new QStandardItem("Type:");
	item0->setToolTip(QString("Name of the module output."));
	item1->setToolTip(QString("Data type of the module output."));
	modelStepOutputs_->setHorizontalHeaderItem(0, item0);
	modelStepOutputs_->setHorizontalHeaderItem(1, item1);
	modelStepOutputs_->setHorizontalHeaderItem(2, new QStandardItem(""));

	mw->ui->tableOutputs->setModel(modelStepOutputs_);
	mw->ui->tableOutputs->setEnabled(false);
	mw->ui->tableOutputs->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	mw->ui->tableOutputs->verticalHeader()->setVisible(false);
	mw->ui->tableOutputs->setSelectionMode(QAbstractItemView::NoSelection);
	for (int c = 0; c < mw->ui->tableOutputs->horizontalHeader()->count(); ++c) {
		mw->ui->tableOutputs->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
	}

}

void RunControl::registerWorkerSlots(RunWorkerThread *wt)
{
	connect(wt, SIGNAL(eventStepActive(std::string)),
	        this, SLOT (on_workerStepActive(std::string)));
	connect(wt, SIGNAL(eventDataUpdated(std::string, std::string)),
	        this, SLOT (on_workerDataUpdated(std::string, std::string)));
	connect(wt, SIGNAL(eventDataDeleted(std::string, std::string)),
	        this, SLOT (on_workerDataDeleted(std::string, std::string)));
}

void RunControl::on_workerStepActive(std::string stepName)
{
	UIPF_LOG_DEBUG("step active: ", stepName);

	steps_.push_back(stepName);
	stepOutputs_.insert(pair<string, vector<string> >(stepName, vector<string>()));

	int row = modelRunSteps_->rowCount();
	modelRunSteps_->insertRow(row);
	QModelIndex index = modelRunSteps_->index(row);
	modelRunSteps_->setData(index, QString::fromStdString(stepName));

	// select newly inserted item
//	mainWindow_->ui->listRunSteps->setCurrentIndex(index);
}

void RunControl::on_workerDataUpdated(std::string stepName, std::string outputName)
{
	UIPF_LOG_DEBUG("on_workerDataUpdated: ", stepName, outputName);

	auto pos = stepOutputs_.find(stepName);
	if (pos != stepOutputs_.end()) {
		// insert output name to the list if it did not exist
		if (find(pos->second.begin(), pos->second.end(), outputName) == pos->second.end()) {
			pos->second.push_back(outputName);

			// TODO if step is selected ,update UI
		}
	}
}

void RunControl::on_workerDataDeleted(std::string stepName, std::string outputName)
{
	UIPF_LOG_DEBUG("on_workerDataDeleted: ", stepName, outputName);

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
			for(string output: outputData->second) {
				QStandardItem* item = new QStandardItem(output.c_str());
				item->setEditable(false);
				// TODO set tooltip from model data description
				modelStepOutputs_->setItem(r, 0, item);
				modelStepOutputs_->setItem(r, 1, new QStandardItem("TODO"));
				mainWindow_->ui->tableOutputs->setIndexWidget(
					mainWindow_->ui->tableOutputs->model()->index(r, 2),
					new QPushButton(QString::fromStdString(string("TODO Buttons")))
				);
			}
		}
		mainWindow_->ui->tableOutputs->setEnabled(true);
	}
}