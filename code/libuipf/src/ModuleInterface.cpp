#include "ModuleInterface.hpp"

using namespace uipf;

std::string ModuleInterface::getProcessingStepName() const {
	return pStepName_;
}

// check whether named input data is given
bool ModuleInterface::hasInputData( const std::string& strName) const {
	return input_.find(strName) != input_.end();
};

void ModuleInterface::updateProgress(int done, int max) {

	runner_->updateModuleProgress(done, max);

}
