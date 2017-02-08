#include "logging.hpp"
#include <unistd.h>
#include "data/opencv.hpp"

#define UIPF_MODULE_ID "uipf.dummy.LongRunningDummyModule"
#define UIPF_MODULE_NAME "LongRunningDummyModule"
#define UIPF_MODULE_CATEGORY "test"
#define UIPF_MODULE_DESCRIPTION "A dummy module for testing long running tasks"
#define UIPF_MODULE_CLASS LongRunningDummyModule

#define UIPF_MODULE_INPUTS \
		{"image", uipf::DataDescription(uipf::data::OpenCVMat::id(), "an image.")}

#define UIPF_MODULE_OUTPUTS \
		{"image", uipf::DataDescription(uipf::data::OpenCVMat::id(), "the same image again :)")}

#define UIPF_MODULE_PARAMS \
		{"steps", uipf::ParamDescription("number of dummy iterations.", true) }

#include "Module.hpp"


void LongRunningDummyModule::run() {

	using namespace cv;
	using namespace uipf;
	using namespace uipf::data;

	OpenCVMat::ptr image;
	if (hasInputData("image")) {
		image = getInputData<OpenCVMat>("image");
	}

	int steps = getParam<int>("steps", 10);

	for(int c = 0; c < steps; c++)
	{
		UIPF_LOG_INFO("I am soo busy!");

// TODO
//		if (context_->isStopRequested())
//		{
//			UIPF_LOG_INFO("Hm, somebody wants me to stop doing my hard, hard work!");
//			UIPF_LOG_INFO("giving up...");
//			break;
//		}
		for(int i = 0; i < 10; ++i) {
			updateProgress(c * 10 + i, steps * 10);
			usleep(100000);
		}

	}

	if (hasInputData("image")) {
		setOutputData<OpenCVMat>("image", image);
	}
}
