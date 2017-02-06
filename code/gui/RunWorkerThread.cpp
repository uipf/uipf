#include "RunWorkerThread.h"

using namespace uipf;

RunWorkerThread::RunWorkerThread(ProcessingChain& chain, ModuleLoader& ml)
 : runner_(Runner(chain, ml))
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
