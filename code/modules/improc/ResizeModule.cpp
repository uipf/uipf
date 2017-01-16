#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "logging.hpp"
#include "exceptions.hpp"
#include "data/std.hpp"
#include "data/opencv.hpp"


#define UIPF_MODULE_ID "uipf.opencv.imgproc.resize"
#define UIPF_MODULE_NAME "openCV Resize Image"
#define UIPF_MODULE_CATEGORY "opencv"
#define UIPF_MODULE_CLASS ResizeImage

#define UIPF_MODULE_INPUTS \
		{"image", uipf::DataDescription(uipf::data::OpenCVMat::id(), "the loaded image.")}

#define UIPF_MODULE_OUTPUTS \
		{"image", uipf::DataDescription(uipf::data::OpenCVMat::id(), "the loaded image.")}

#define UIPF_MODULE_PARAMS \
		{"width", uipf::ParamDescription("new width, optional, will be determines by height using aspect ratio.", true) }, \
		{"height", uipf::ParamDescription("new height, optional, will be determines by width using aspect ratio.", true) }, \
		{"max_width", uipf::ParamDescription("max width, image will only be resized if it is bigger than this.", true) }, \
		{"max_height", uipf::ParamDescription("max height, image will only be resized if it is bigger than this.", true) }

#include "Module.hpp"

// TODO write tests for this

void ResizeImage::run() {

	using namespace cv;
	using namespace uipf;
	using namespace uipf::data;

	OpenCVMat::ptr image = getInputData<OpenCVMat>("image");

	// read the params (width and height) given for this step
	int width = getParam<int>("width", -1);
	int height = getParam<int>("height", -1);
	int max_width = getParam<int>("max_width", -1);
	int max_height = getParam<int>("max_height", -1);
	bool do_resize = false;

	// set parameters that where not given, calculate by aspect ration
	if (width > 0 || height > 0) {
		// TODO do not fail hard!
		assert(max_width == -1 && max_height == -1);

		// calculate aspect ratio
		if (width <= 0) {
			width = image->getContent().cols / image->getContent().rows * height;
		}
		if (height <= 0) {
			height = image->getContent().rows / image->getContent().cols * width;
		}

		do_resize = true;
	} else {
		// at least one of these should be given
		assert(max_width > 0 || max_height > 0);

		width = image->getContent().cols;
		height = image->getContent().rows;

		if (max_width > 0 && width > max_width) {
			height = max_width / width * height;
			width = max_width;
			do_resize = true;
		}
		if (max_height > 0 && height > max_height) {
			width = max_height / height * width;
			height = max_height;
			if (max_width > 0 && width > max_width) {
				height = max_width / width * height;
				width = max_width;
			}
			do_resize = true;
		}
	}

	if (do_resize) {
		// get the actual opencv matrix of the input data
		Mat m = image->getContent();
		// do resize using opencv
		resize(m, m, Size(width, height));
		// set the result (image) on the datamanager
		setOutputData<OpenCVMat>("image", new OpenCVMat(m));
	} else {
		setOutputData<OpenCVMat>("image", image);
	}
}
