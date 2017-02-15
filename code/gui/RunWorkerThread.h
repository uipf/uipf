#ifndef RUNWORKERTHREAD_H_
#define RUNWORKERTHREAD_H_

#include <QThread>

#include "Runner.hpp"

namespace uipf {

//A class that does the background work in a thread.
//By deriving QThread QT handles signals/slot thread marshaling automatically.
//for this all Types used in signals have to be registered by qRegisterMetaType() -> see GUIEventDispatcher.cpp
class RunWorkerThread: public QThread, public RunContext {

	Q_OBJECT

public:

	enum ExecutionMode {
		RUN_CHAIN,
		RUN_STEP,
	};

	RunWorkerThread(ProcessingChain&, ModuleLoader&, ExecutionMode = RUN_CHAIN);
	virtual ~RunWorkerThread();

	void setMode(ExecutionMode);

	//run module chain in a separate thread
	void run() Q_DECL_OVERRIDE;
	//tell modules to stop work now
	void stop();

	void pause();


	void updateGlobalProgress(int done, int max);
	void updateModuleProgress(int done, int max);
	void stepActive(const std::string& stepName, int number, int count);
	void dataUpdated(const std::string& stepName, const std::string& outputName, Data::ptr& data);
	void dataDeleted(const std::string& stepName, const std::string& outputName);

signals: //for QT to connect
	void eventUpdateGlobalProgress(int done, int max);
	void eventUpdateModuleProgress(int done, int max);
	void eventStepActive(std::string stepName, int number, int count);
	void eventDataUpdated(std::string stepName, std::string outputName, Data::ptr data);
	void eventDataDeleted(std::string stepName, std::string outputName);

private:

	Runner runner_;
	ExecutionMode mode_;

};

} // namespace

#endif /* RUNWORKERTHREAD_H_ */
