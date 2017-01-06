#include <iostream>
#include <opencv2/highgui/highgui.hpp>

#include "logging.hpp"
#include "exceptions.hpp"
#include "data/std.hpp"
#include "data/opencv.hpp"


#define UIPF_MODULE_ID "uipf.opencv.load"
#define UIPF_MODULE_NAME "openCV Load Image"
#define UIPF_MODULE_CATEGORY "opencv"
#define UIPF_MODULE_CLASS LoadImage

#define UIPF_MODULE_OUTPUTS \
		{"image", uipf::DataDescription(uipf::data::OpenCVMat::id(), "the loaded image.")}

#define UIPF_MODULE_PARAMS \
		{"filename", uipf::ParamDescription("file name of the file to load from.") }, \
		{"mode", uipf::ParamDescription("can be either 'color' or 'grayscale' for whether to load the image in color or grayscale mode. Defaults to 'color'.", true) }

#include "Module.hpp"

void LoadImage::run() {

	using namespace cv;
	using namespace uipf;
	using namespace uipf::data;

	Mat image;
	std::string strFilename = getParam<std::string>("filename","");
	// check whether to load the image in grayscale mode, defaults to color
	if (getParam<std::string>("mode", "color").compare("grayscale") == 0) {
		image = imread (strFilename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	} else {
		image = imread (strFilename.c_str(), CV_LOAD_IMAGE_COLOR);
	}

	if (!image.data) { // Check for invalid input
		throw ErrorException(string("Could not open or find the image: ") + strFilename);
	}
	OpenCVMat::ptr mat(new OpenCVMat(image));
	mat->filename = strFilename;
	setOutputData<OpenCVMat>("image", mat);
	UIPF_LOG_INFO("loaded image" + mat->filename);
}
