#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/filesystem.hpp>

#include "logging.hpp"
#include "exceptions.hpp"
#include "data/std.hpp"
#include "data/opencv.hpp"
#include "util.hpp"


#define UIPF_MODULE_ID "uipf.opencv.imgproc.copy"
#define UIPF_MODULE_NAME "openCV Copy Image"
#define UIPF_MODULE_CATEGORY "opencv"
#define UIPF_MODULE_CLASS CopyImageModule

#define UIPF_MODULE_INPUTS \
		{"image", uipf::DataDescription(uipf::data::OpenCVMat::id(), "the source image.")}

#define UIPF_MODULE_OUTPUTS \
		{"image", uipf::DataDescription(uipf::data::OpenCVMat::id(), "the copied image.")}

#define UIPF_MODULE_PARAMS \
		{"path", uipf::ParamDescription("the target path to copy to.") }, \
		{"overwrite", uipf::ParamDescription("whether to overwrite existing files. Defaults to false.", true) }, \
		{"lowercase", uipf::ParamDescription("whether to convert the file name to lower case. Defaults to false.", true) }

		// TODO add options to rename file

#include "Module.hpp"

// TODO write tests for this

void CopyImageModule::run() {

	using namespace std;
	using namespace uipf;
	using namespace uipf::data;

	namespace fs = boost::filesystem;

	OpenCVMat::ptr image = getInputData<OpenCVMat>("image");
	fs::path path = getParam<string>("path", ".");

	fs::path src = image->filename;

	fs::path target = path;
	if (!fs::exists(target)) {
		fs::create_directories(target);
	}
	if (getParam<bool>("lowercase", false)) {
		target /= util::str_to_lower(src.filename().string());
	} else {
		target /= src.filename();
	}

	if (!fs::exists(src)) {
		throw ErrorException(string("Source file does not exist: ") + src.string());
	}
	if (fs::exists(target)) {
		if (getParam<bool>("overwrite", false)) {
			fs::remove(target);
		} else {
			throw ErrorException(string("Target file already exists: ") + src.string());
		}
	}
	// TODO error handling
	fs::copy(src, target);
	image->filename = target.string();

	setOutputData<OpenCVMat>("image", image);
}
