#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include <QScrollBar>
#include <iostream>
#include "../framework/ModuleManager.hpp"

using namespace std;
using namespace uipf;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);

    // Create model
    modelStep = new QStringListModel(this);
    modelModule = new QStringListModel(this);
    modelTableParams = new ProcessingStepParams(this);
    modelTableInputs = new ProcessingStepInputs(this);

    // Glue model and view together
    ui->listProcessingSteps->setModel(modelStep);
    ui->tableParams->setModel(modelTableParams);
    ui->tableInputs->setModel(modelTableInputs);
	ui->comboModule->setModel(modelModule);

    // Add additional feature so that
    // we can manually modify the data in ListView
    // It may be triggered by hitting any key or double-click etc.
    ui->listProcessingSteps->
            setEditTriggers(QAbstractItemView::AnyKeyPressed |
                            QAbstractItemView::DoubleClicked);


	// react to changes in the ListView
	// TODO improve this to react on any selection change: http://stackoverflow.com/questions/2468514/how-to-get-the-selectionchange-event-in-qt
    connect(ui->listProcessingSteps, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(on_listProcessingSteps_activated(const QModelIndex &)));
            
            
    // commands for menu bar     
    createActions();
    createMenus();
	// sets undo and redo to inactive
    undoAct->setEnabled(false);
    redoAct->setEnabled(false);
    // sets run and stop to inactive
    runAct->setEnabled(false);
    stopAct->setEnabled(false);
    
    
    
    setWindowTitle(tr("uipf"));
    setMinimumSize(160, 160);
    resize(480, 320);
    
    
    
    

    connect(Logger::instance(), SIGNAL (logEvent(const Logger::LogType&,const std::string&)), this, SLOT (on_appendToLog(const Logger::LogType&,const std::string&)));
}

void MainWindow::on_appendToLog(const Logger::LogType& eType,const std::string& strText) {
	// For colored Messages we need html :-/
	QString strColor = (eType == Logger::WARNING ? "Blue" : eType == Logger::ERROR ? "Red" : "Green");
	QString alertHtml = "<font color=\""+strColor+"\">" + QString(strText.c_str()) + "</font>";
	ui->tbLog->appendHtml(alertHtml);
	ui->tbLog->verticalScrollBar()->setValue(ui->tbLog->verticalScrollBar()->maximum());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete modelStep;
    delete modelModule;
    delete modelTableParams;
    delete modelTableInputs;
}


// sets a Module list
void MainWindow::setModuleList(QStringList list){
	modelModule->setStringList(list);
}

// Add button clicked
// Adding at the end
void MainWindow::on_addButton_clicked() {
    // Get the position of the selected item
    int row = modelStep->rowCount();

    // Enable add one or more rows
    modelStep->insertRows(row,1);

    // Get the row for Edit mode
    QModelIndex index = modelStep->index(row);

    // Enable item selection and put it edit mode
    ui->listProcessingSteps->setCurrentIndex(index);
    ui->listProcessingSteps->edit(index);
}

// Delete button clicked
void MainWindow::on_deleteButton_clicked() {
    // Get the position
    modelStep->removeRows(ui->listProcessingSteps->currentIndex().row(),1);
}

// gets called when a processing step is selected
void MainWindow::on_listProcessingSteps_activated(const QModelIndex & index) {

	map<string, ProcessingStep> chain = conf_.getProcessingChain();

	string selectedStep = ui->listProcessingSteps->model()->data(ui->listProcessingSteps->currentIndex()).toString().toStdString();
	cout << "selected " << selectedStep << endl;
	ProcessingStep proStep = chain[selectedStep];

	modelTableParams->setProcessingStep(proStep);
	modelTableInputs->setProcessingStep(proStep);
}

void MainWindow::new_Data_Flow() {
	// run is now activated
	runAct->setEnabled(true);    
	
    // configuration changed
	configChanged();

    Configuration conf;
    conf_ = conf;
    
    QStringList list;
	modelStep->setStringList(list);
}




void MainWindow::load_Data_Flow() {
	// run is now activated
	runAct->setEnabled(true);    

    // configuration changed
	configChanged();

	QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),QString(), tr("YAML-Files (*.yaml);;All Files (*)"));
  	currentFileName = fn.toStdString();

	conf_.load(currentFileName);

    // only for debug, print the loaded config
	conf_.print();

	// set the names of the processing steps:
	QStringList list;
	map<string, ProcessingStep> chain = conf_.getProcessingChain();
	for (auto it = chain.begin(); it!=chain.end(); ++it) {
		list << it->first.c_str();
	}
	modelStep->setStringList(list);
	
}


void MainWindow::save_Data_Flow() {
	conf_.store(currentFileName);
}

void MainWindow::save_Data_Flow_as() {
	
	QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                              QString(), tr("YAML files (*.yaml);;All Files (*)"));
    if (! (fn.endsWith(".yaml", Qt::CaseInsensitive)) )
        fn += ".yaml"; // default
  	currentFileName = fn.toStdString();
	conf_.store(fn.toStdString());
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About uipf"),
            tr("This application allows the user by using given library of modules to create his own configuration and to execute it. Created by the project: 'A Unified Framework for Digital Image Processing in Computer Vision and Remote Sensing' at Technische Universitaet Berlin. Version 1.0"));
}

// sets the current configuration to the previous one
void MainWindow::undo() {
	if(!undoStack.empty()){
		// move the current config to the redo stack
		redoStack.push(conf_);
		// get the last config stored in undo stack
		conf_ = undoStack.top();
		// dele the last config from the undo stack
		undoStack.pop();

		// set the names of the processing steps:
		QStringList list;
		map<string, ProcessingStep> chain = conf_.getProcessingChain();
		for (auto it = chain.begin(); it!=chain.end(); ++it) {
			list << it->first.c_str();
		}
		modelStep->setStringList(list);
		
		// set the undo/redo in the menu bar gray if inactive or black, if active	
		redoAct->setEnabled(true);	
		if(!undoStack.empty()){
			undoAct->setEnabled(true);
		} else{
			undoAct->setEnabled(false);
		}
	}
}

// sets the current configuration back to the next one
void MainWindow::redo() {
	if(!redoStack.empty()){
		// move the current config to the undo stack
		undoStack.push(conf_);
		// get the last config stored in redo stack
		conf_ = redoStack.top();
		// dele the last config from the redo stack
		redoStack.pop();
		
		// set the names of the processing steps:
		QStringList list;
		map<string, ProcessingStep> chain = conf_.getProcessingChain();
		for (auto it = chain.begin(); it!=chain.end(); ++it) {
			list << it->first.c_str();
		}
		modelStep->setStringList(list);
	
		// set the undo/redo in the menu bar gray if inactive or black, if active
		undoAct->setEnabled(true);	
		if(!redoStack.empty()){
			redoAct->setEnabled(true);
		} else{
			redoAct->setEnabled(false);
		}
	}
}

// feeds the undo and redo stacks with the current configs
void MainWindow::configChanged(){
	// configuration changed
	undoStack.push(conf_);
	while(! redoStack.empty()){
		redoStack.pop();
	}
	
	// set the undo to active and redo to inactive
    undoAct->setEnabled(true);
    redoAct->setEnabled(false);
}

// run the current configuration
void MainWindow::run() {
	// stop is now activated
	stopAct->setEnabled(true);    

	ModuleManager mm;
	mm.run(conf_);
	
	// TODO inactivate stop, when finished!
}

void MainWindow::stop() {
	// TODO
}


void MainWindow::createActions() {
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new configuration"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(new_Data_Flow()));
    
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing configuration"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(load_Data_Flow()));
    
    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the configuration to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save_Data_Flow()));
    
    saveAsAct = new QAction(tr("Save &as..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the configuration to disk"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(save_Data_Flow_as()));
    
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setShortcuts(QKeySequence::WhatsThis);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    
    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last operation"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last operation"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    runAct = new QAction(tr("&Run"), this);
    runAct->setStatusTip(tr("Run the configuration"));
    connect(runAct, SIGNAL(triggered()), this, SLOT(run()));

    stopAct = new QAction(tr("&Stop"), this);
    stopAct->setStatusTip(tr("Stop the execution of the configuration"));
    connect(stopAct, SIGNAL(triggered()), this, SLOT(stop()));
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    
    configMenu = menuBar()->addMenu(tr("&Configuration"));
    configMenu->addAction(runAct);
    configMenu->addAction(stopAct);
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);

}

