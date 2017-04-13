#ifndef UIPFRAMEWORK_RUNCONTEXT_HPP
#define UIPFRAMEWORK_RUNCONTEXT_HPP

namespace uipf {

	class RunContext {
	public:
		/**
		 * Update overall processing chain progress information.
		 */
		virtual void updateGlobalProgress(int done, int max) {};

		/**
		 * Update module progress information.
		 */
		virtual void updateModuleProgress(int done, int max) {};

		/**
		 * Mark currently active processing step.
		 *
		 * @param stepName name of the processing step.
		 * @param number the ordered number of this step.
		 * @param count count of all steps. This is used to display a "i of n steps" have finished in the GUI.
		 */
		virtual void stepActive(const std::string& stepName, int number, int count) {};

		/**
		 * Mark processin step as finished.
		 * @param stepName name of the processing step.
		 * @param success set to false on failure.
		 */
		virtual void stepFinished(const std::string& stepName, bool success) {};

		/**
		 * Notify about new data available for processing step
		 * @param stepName name of the processing step.
		 * @param outputName name of the step output.
		 * @param data pointer to the data.
		 */
		virtual void dataUpdated(const std::string& stepName, const std::string& outputName, Data::ptr &data) {};

		/**
		 * Notify about deletion of data.
		 * @param stepName name of the processing step.
		 * @param outputName name of the step output.
		 * @deprecated TODO this is not really needed anymore.
		 */
		virtual void dataDeleted(const std::string& stepName, const std::string& outputName) {};

		bool stopRequested = false;
		bool pauseRequested = false;
	};

}


#endif //UIPFRAMEWORK_RUNCONTEXT_HPP
