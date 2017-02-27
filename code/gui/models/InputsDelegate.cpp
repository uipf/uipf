#include "ModuleInterface.hpp"
#include "InputsDelegate.hpp"

#include <QWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QModelIndex>
#include <QApplication>
#include <QString>

#include <iostream>
#include <data/list.hpp>

using namespace std;
using namespace uipf;

InputsDelegate::InputsDelegate(ModuleLoader& mm, QObject *parent) : QItemDelegate(parent) , mm_(mm) {

}

void InputsDelegate::setConfiguration(const ProcessingChain& conf, const std::string& currentStepName, std::vector<std::string> inputNames) {

	// populate names of the current modules inputs
	inputNames_ = inputNames;

	stepItems_.clear();
	outputItems_.clear();

	map<string, ModuleMetaData> moduleMetaData = mm_.getAllMetaData();

	// fill vector of possible reference steps
	map<string, ProcessingStep> chain = conf.getProcessingSteps();
	for (auto it = chain.begin(); it!=chain.end(); ++it) {
		// the current step can not depend on itself
		if (it->first.compare(currentStepName) == 0) {
			continue;
		}
		// also only steps are listed that have at least one output
		auto metaIt = moduleMetaData.find(it->second.module);
		if (metaIt != moduleMetaData.end() && metaIt->second.getOutputs().empty()) {
			continue;
		}

		stepItems_.push_back(it->first.c_str());
	}

	// populate outputs for selection
	ProcessingStep step = conf.getProcessingStep(currentStepName);
	for(auto it = step.inputs.cbegin(); it != step.inputs.end(); ++it) {

		// fill optional inputs
		auto ownMetaIt = moduleMetaData.find(step.module);
		if (ownMetaIt != moduleMetaData.end()) {
			map<string, DataDescription> in = ownMetaIt->second.getInputs();
			for (auto iit = in.cbegin(); iit!=in.end(); ++iit) {
				if (iit->second.getIsOptional()) {
					optionalInputs_.push_back(iit->first);
				}
			}
		}

		// fill vector of possible outputs for each referenced module
		vector<string> subItems;
		if (conf.hasProcessingStep(it->second.sourceStep)) {
			// only possible to add items if the referenced step exists and we can get its module
			ProcessingStep referencedStep = conf.getProcessingStep(it->second.sourceStep);

			// fill vector of possible output selections if module exists
			auto metaIt = moduleMetaData.find(referencedStep.module);
			if (metaIt != moduleMetaData.end()) {
				map<string, DataDescription> out = metaIt->second.getOutputs();
				for (auto oit = out.cbegin(); oit!=out.end(); ++oit) {
					subItems.push_back(oit->first);
					// add a .map() version of the output, if it is a list
					if (oit->second.getType() == uipf::data::List::id() || referencedStep.isMapping) {
						subItems.push_back(oit->first + string(".map()"));
					}
				}
			}
		}
		outputItems_.push_back(subItems);
	}

}

// create editor widget
QWidget *InputsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex & index) const {
	QComboBox* editor = new QComboBox(parent); // will be deleted by Qt automatically http://doc.qt.io/qt-5/qabstractitemdelegate.html#destroyEditor

	// first column is the processing step
	if (index.column() == 0) {

		int k = 0;
		// add a selection that is empty for optional inputs
		string inputName = inputNames_[index.row()];
		for (unsigned int i = 0; i < optionalInputs_.size(); ++i) {
			if (inputName.compare(optionalInputs_[i]) == 0) {
				editor->insertItem(k++, QString(""), QString(""));
				break;
			}
		}
		// fill list with available steps
		for(unsigned int i = 0; i < stepItems_.size(); ++i) {
			editor->insertItem(k++, QString(stepItems_[i].c_str()), QString(stepItems_[i].c_str()));
		}
	}
	// second column is the output of the source module
	else if (index.column() == 1) {
		vector<string> items = outputItems_[index.row()];
		for(unsigned int i = 0; i < items.size(); ++i) {
			editor->insertItem(i, QString(items[i].c_str()), QString(items[i].c_str()));
		}
	}
	return editor;
}

// set column content for edit-mode
void InputsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
	QComboBox *comboBox = static_cast<QComboBox*>(editor);
	int value = comboBox->findData(index.data());
	comboBox->setCurrentIndex(value);
}

// set column content for non edit-mode
void InputsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
	QComboBox *comboBox = static_cast<QComboBox*>(editor);
	string selectedItem = comboBox->currentData().toString().toStdString();
	model->setData(index, QString(selectedItem.c_str()));

	//cout << "set model data " << index.row() << "," << index.column() << " : " << selectedItem << endl;

	pair<string, string> newInput(
		model->data(model->index(index.row(), 0)).toString().toStdString(),
		model->data(model->index(index.row(), 1)).toString().toStdString()
	);
	emit inputChanged(inputNames_[index.row()], newInput);
}
