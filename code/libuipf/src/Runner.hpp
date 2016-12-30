#ifndef LIBUIPF_RUNNER_HPP
#define LIBUIPF_RUNNER_HPP

#include "ProcessingChain.hpp"
#include "ModuleLoader.hpp"
#include "ModuleInterface.hpp"

namespace uipf{

class Runner{

	public:
		// constructor
		Runner(const ProcessingChain& pc, ModuleLoader& ml) :
			processingChain_(pc),
			moduleLoader_(ml) {};

		// destructor
		~Runner(void) {};

		/**
		 * Runs the processing chain by invoking the modules
		 * with given parameters.
		 */
		void run();

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

};

} // namespace

#endif // LIBUIPF_RUNNER_HPP
