#ifndef LIBUIPF_RUNNER_HPP
#define LIBUIPF_RUNNER_HPP

#include "ProcessingChain.hpp"
#include "ModuleLoader.hpp"
#include "ModuleInterface.hpp"
#include "RunContext.hpp"

namespace uipf{

/**
 * Runner implements the class that brings Modules and Data together in a ProcessingChain.
 *
 * It takes the ProcessingChain and a ModuleLoader to run the chain by executing the modules.
 *
 * There are two run modi:
 *
 * 1. Run the whole chain. Simply call `run()` to do so. Call `pauseChain()` to reduce a run to step mode.
 * 2. Run step by step. Start with `runStep()`. Call `resumeChain()` to change to whole run mode.
 *
 * Abort a running chain with `requestStop()`.
 * TODO this is untested so far.
 *
 *
 *
 *
 * Runner assumes the processing chain has been validated before, do not pass an invalid chain!
 */
class Runner{

	public:
		// constructor
		Runner(const ProcessingChain &pc, ModuleLoader &ml, RunContext &ct);

		// destructor
		~Runner(void);

		enum DataMode {
			// keep data as long as there are modules that need them
			// TODO allow marking some data for explicit save
			MODE_ECONOMY,
			// keep all data for later inspection
			MODE_KEEP,
		};

		DataMode dataMode = MODE_ECONOMY;

		/**
		 * Runs the processing chain by invoking the modules
		 * with given parameters.
		 */
		bool run();

		bool runStep();

		/**
		 * if set modules should finish their work
		 */
		void requestStop();

		/**
		 * set pause and resume to enable blocking windows in GUI mode
		 */
		void requestPause();

		void resumeChain();


	private:

		void cleanupData();

		const ProcessingChain &processingChain_;
		ModuleLoader &moduleLoader_;
		RunContext &context_;

		std::map<std::string, uipf::ProcessingStep> chain_;

		unsigned int currentStep_ = 0;
		std::vector<std::string> sortedChain_;

		// contains the outputs of the processing steps
		std::map<std::string, std::map<std::string, uipf::Data::ptr> > stepsOutputs_;


		int moduleProgressDone = 0;
		int moduleProgressMax = 100;
		int modulesDone = 0;
		int moduleCount = 0;
		int mapDone = 0;
		int mapItems = 0;

		std::vector<std::string> sortChain(const std::map<std::string, ProcessingStep> &);

	private:

		void stepActive(std::string stepName, int number, int count);

		void dataUpdated(std::string stepName, std::string outputName, Data::ptr data);

		void dataDeleted(std::string stepName, std::string outputName);

		void updateModuleProgress(int done, int max = 100);

		friend class ModuleInterface;


	};

} // namespace

#endif // LIBUIPF_RUNNER_HPP
