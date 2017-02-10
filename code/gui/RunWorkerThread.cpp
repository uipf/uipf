#include "ModuleInterface.hpp"
#include "RunWorkerThread.h"
#include "logging.hpp"

using namespace uipf;

RunWorkerThread::RunWorkerThread(ProcessingChain& chain, ModuleLoader& ml, ExecutionMode mode)
 : runner_(Runner(chain, ml, *this)),
   mode_(mode)
{

}

RunWorkerThread::~RunWorkerThread()
{
	UIPF_LOG_TRACE("RunWorkerThread destructor.");
}

void RunWorkerThread::stop()
{
	UIPF_LOG_TRACE("RunWorkerThread stop()");
	stopRequested = true;
}

void RunWorkerThread::run()
{
	if (mode_ == RUN_CHAIN) {
		runner_.run();
	} else {
		runner_.runStep();
	}
	// TODO error handling
}

void RunWorkerThread::pause() {
	pauseRequested = true;
}


void RunWorkerThread::updateGlobalProgress(int done, int max)
{
	emit eventUpdateGlobalProgress(done, max);
}

void RunWorkerThread::updateModuleProgress(int done, int max)
{
	emit eventUpdateModuleProgress(done, max);
}

void RunWorkerThread::stepActive(std::string stepName, int number, int count)
{
	emit eventStepActive(stepName, number, count);
}
void RunWorkerThread::dataUpdated(std::string stepName, std::string outputName)
{
	emit eventDataUpdated(stepName, outputName);
}
void RunWorkerThread::dataDeleted(std::string stepName, std::string outputName)
{
	emit eventDataDeleted(stepName, outputName);
}

void RunWorkerThread::setMode(RunWorkerThread::ExecutionMode mode) {
	mode_ = mode;
}
