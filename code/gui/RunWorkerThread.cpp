#include "RunWorkerThread.h"

using namespace uipf;

RunWorkerThread::RunWorkerThread(ProcessingChain& chain, ModuleLoader& ml)
 : runner_(Runner(chain, ml, *this))
{

}

RunWorkerThread::~RunWorkerThread() {
}

void RunWorkerThread::stop()
{
// TODO
//	runner_.requestStop();
}

void RunWorkerThread::run()
{
	runner_.run();
}


void RunWorkerThread::updateGlobalProgress(int done, int max)
{
	emit eventUpdateGlobalProgress(done, max);
}

void RunWorkerThread::updateModuleProgress(int done, int max)
{
	emit eventUpdateModuleProgress(done, max);
}

void RunWorkerThread::stepActive(std::string stepName)
{
	emit eventStepActive(stepName);
}
void RunWorkerThread::dataUpdated(std::string stepName, std::string outputName)
{
	emit eventDataUpdated(stepName, outputName);
}
void RunWorkerThread::dataDeleted(std::string stepName, std::string outputName)
{
	emit eventDataDeleted(stepName, outputName);
}
