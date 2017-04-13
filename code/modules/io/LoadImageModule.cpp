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
		{"mode", uipf::ParamDescription("can be either 'color' or 'grayscale' for whether to load the image in color or grayscale mode. Defaults to 'color'.", true) }, \
		{"exif", uipf::ParamDescription("whether to load EXIF data. Defaults to 'yes'.", true) }

#include "Module.hpp"

void LoadImage::run() {

	using namespace cv;
	using namespace uipf;
	using namespace uipf::data;

	std::string strFilename = getParam<std::string>("filename","");

	OpenCVMat::ptr mat;

	// check whether to load the image in grayscale mode, defaults to color
	if (getParam<std::string>("mode", "color").compare("grayscale") == 0) {
		mat = load_image_greyscale(strFilename.c_str());
	} else {
		mat = load_image_color(strFilename.c_str());
	}
	if (getParam<bool>("exif", true)) {
		mat->exif = load_image_exif_data(strFilename);
	}
	setOutputData<OpenCVMat>("image", mat);
	UIPF_LOG_INFO("loaded image" + mat->filename);
}
