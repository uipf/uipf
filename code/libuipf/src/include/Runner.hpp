#ifndef LIBUIPF_RUNNER_HPP
#define LIBUIPF_RUNNER_HPP

#include "ProcessingChain.hpp"
#include "ModuleLoader.hpp"
#include "ModuleInterface.hpp"
#include "RunContext.hpp"

namespace uipf{

class Runner{

	public:
		// constructor
		Runner(const ProcessingChain& pc, ModuleLoader& ml, RunContext& ct) :
			processingChain_(pc),
			moduleLoader_(ml),
			context_(ct) {};

		// destructor
		~Runner(void) {};

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

		/**
		 * if set modules should finish their work
		 */
		void requestStop();

		/**
		 * set pause and resume to enable blocking windows in GUI mode
		 */
		void pauseChain();
		void resumeChain();


	private:

		const ProcessingChain& processingChain_;
		ModuleLoader& moduleLoader_;
		RunContext& context_;

		int moduleProgressDone = 0;
		int moduleProgressMax = 100;
		int modulesDone = 0;
		int moduleCount = 0;
		int mapDone = 0;
		int mapItems = 0;

		std::vector<std::string> sortChain(const std::map<std::string, ProcessingStep>&);

	private:

		void stepActive(std::string stepName);
		void dataUpdated(std::string stepName, std::string outputName);
		void dataDeleted(std::string stepName, std::string outputName);
		void updateModuleProgress(int done, int max = 100);

		friend class ModuleInterface;


};

} // namespace

#endif // LIBUIPF_RUNNER_HPP
