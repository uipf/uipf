#ifndef UIPFRAMEWORK_RUNCONTEXT_HPP
#define UIPFRAMEWORK_RUNCONTEXT_HPP

namespace uipf {

	class RunContext {
	public:
		virtual void updateGlobalProgress(int done, int max) {};

		virtual void updateModuleProgress(int done, int max) {};

		virtual void stepActive(const std::string& stepName, int number, int count) {};

		virtual void dataUpdated(const std::string& stepName, const std::string& outputName, Data::ptr &data) {};

		virtual void dataDeleted(const std::string& stepName, const std::string& outputName) {};

		bool stopRequested = false;
		bool pauseRequested = false;
	};

}


#endif //UIPFRAMEWORK_RUNCONTEXT_HPP
