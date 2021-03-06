#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QListWidgetItem>
#include <QStandardItemModel>

#include <QDialog>
#include <QLabel>
#include <QStatusBar>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QMessageBox>

#include <stack>

#include "models/ParamsModel.hpp"
#include "models/InputsDelegate.hpp"

#include "RunWorkerThread.h"

#include "ProcessingChain.hpp"
#include "logging.hpp"

#include "graph/graphwidget.h"
#include "graph/node.h"
#include "RunControl.hpp"
#include "GuiVisualizationContext.hpp"


namespace Ui {
	class MainWindow;
}

namespace uipf {

class GuiLogger;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ModuleLoader& ml, QWidget *parent = 0);
    ~MainWindow();

	// loads a new configuration from file
	void loadProcessingChain(std::string);

	bool validateChain();

private slots:
	// Buttons addStep/deleteStep
    void on_addButton_clicked();
    void on_deleteButton_clicked();
	//stepName changed
	void on_stepNameChanged();

    // Activation of Step (via clicking)
    void on_stepSelectionChanged(const QItemSelection&);

    // append messages from our logger to the log-textview
    void on_appendToLog(log::Logger::LogLevel, const std::string& );

    // moves the progressbar on every step of the processing chain
    void on_reportModuleProgress(int done, int max);
    void on_reportGlobalProgress(int done, int max);

	// change of module dropdown
	void on_comboModule_currentIndexChanged(int);

	// change of module category dropdown
	void on_comboCategory_currentIndexChanged(int);

	// change in the params table
	void on_paramChanged(std::string, std::string);
	void on_inputChanged(std::string, std::pair<std::string, std::string>);

	void on_createImageWindow(const std::string& strTitle);
	void on_createTextWindow(const std::string& strTitle, const std::string& text);

	// menu bar
	// File
	void new_Data_Flow();
	void load_Data_Flow();
	void save_Data_Flow();
	void save_Data_Flow_as();
	void on_close();
	// Help
	void about();
	// Edit
	void undo();
	void redo();

	void closeAllCreatedWindows();

	void on_graphNodeSelected(const uipf::gui::Node* node);

	void on_clearLogButton_clicked();

	void on_logFiltertextChanged(const QString& text);

protected:
	void closeEvent(QCloseEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

private:
	// default window title that appears next to the file name
	const std::string WINDOW_TITLE = "uipf";

    Ui::MainWindow *ui;
	friend class RunControl;

	// the module manager instance
	ModuleLoader& mm_;

	// model for the listView of processing steps
    QStringListModel *modelStep;
	// model for the params editor table
    ParamsModel *modelTableParams;
	// models for the input editor table
    QStandardItemModel *modelTableInputs;
    // model delegate for the input editor table
	InputsDelegate *delegateTableInputs;

	// the file name of the currently loaded configuration
	std::string currentFileName;
	// asks the user, whether he wants to save the file
	bool okToContinue();
	// the currently loaded configuration represented in the window
   	ProcessingChain conf_;

	// map of all available categories
	std::map<std::string, std::vector<std::string> > categories_;

	// counts the undo/redo, when = 0, it is the saved version
	int savedVersion = 1;
	// is true if file was at least one time saved
	bool unknownFile = true;

	// current name of a precessing step
	std::string currentStepName;

   	// Redo and Undo stacks, which store configurations
   	std::stack<ProcessingChain> undoStack;
   	std::stack<ProcessingChain> redoStack;
	// fills the undo and redo stacks
	void beforeConfigChange();

	// refresh UI triggers
	void refreshCategoryAndModule();
	void refreshParams();
	void refreshInputs();
	void refreshGraph();
	void refreshSaveIcon();

	// reset UI triggers
	void resetCategoryAndModule();
	void resetParams();
	void resetInputs();

	// menu bar
    void createActions();
    void deleteActions();
    void createMenus();

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *configMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    // actions in fileMenu
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;

    // actions in editMenu
   	QAction *undoAct;
    QAction *redoAct;

    // actions in configMenu
   	QAction *runAct;
    QAction *stopAct;

    // actions in viewMenu
    QAction *closeWindowsAct;

    // actions in helpMenu
    QAction *aboutAct;

    //the view, that displays the graph
    gui::GraphWidget* graphView_;

	RunControl* runControl;

    //keep track of all windows we created so we can close them later
    std::vector<QWidget* > createdWindwows_;

	GuiVisualizationContext* visualizationContext_;
};

} // namespace

#endif // MAINWINDOW_H
