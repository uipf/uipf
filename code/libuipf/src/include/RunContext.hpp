#ifndef UIPFRAMEWORK_RUNCONTEXT_HPP
#define UIPFRAMEWORK_RUNCONTEXT_HPP

namespace uipf {

	class RunContext
	{
	public:
		virtual void updateGlobalProgress(int done, int max) {};
		virtual void updateModuleProgress(int done, int max) {};

		virtual void stepActive(std::string stepName) {};
		virtual void dataUpdated(std::string stepName, std::string outputName) {};
		virtual void dataDeleted(std::string stepName, std::string outputName) {};
	};

}


#endif //UIPFRAMEWORK_RUNCONTEXT_HPP
