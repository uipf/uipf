#ifndef UIPFRAMEWORK_RUNCONTEXT_HPP
#define UIPFRAMEWORK_RUNCONTEXT_HPP

namespace uipf {

	class RunContext
	{
	public:
		virtual void updateGlobalProgress(int done, int max) {};
		virtual void updateModuleProgress(int done, int max) {};
	};

}


#endif //UIPFRAMEWORK_RUNCONTEXT_HPP
