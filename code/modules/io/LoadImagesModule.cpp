#include <iostream>
#include <opencv2/highgui/highgui.hpp>
// boost filesystem for iterating directories
#include <boost/filesystem.hpp>

#include "logging.hpp"
#include "exceptions.hpp"
#include "data/std.hpp"
#include "data/opencv.hpp"
#include "data/list.hpp"


#define UIPF_MODULE_ID "uipf.opencv.loadImages"
#define UIPF_MODULE_NAME "openCV Load Images from folder"
#define UIPF_MODULE_CATEGORY "opencv"
#define UIPF_MODULE_CLASS LoadImages

#define UIPF_MODULE_OUTPUTS \
		{"imageset", uipf::DataDescription(uipf::data::OpenCVMat::id(), "the loaded images.")}

#define UIPF_MODULE_PARAMS \
		{"path", uipf::ParamDescription("name of the directory to load from.") }, \
		{"mode", uipf::ParamDescription("can be either 'color' or 'grayscale' for whether to load the image in color or grayscale mode. Defaults to 'color'.", true) }

#include "Module.hpp"

using namespace cv;
using namespace uipf;
using namespace uipf::data;

cv::Mat loadimage(const std::string& strFilename, const std::string& mode) {
	Mat image;

	// check whether to load the image in grayscale mode, defaults to color
	if (mode.compare("grayscale") == 0) {
		image = imread (strFilename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	} else {
		image = imread (strFilename.c_str(), CV_LOAD_IMAGE_COLOR);
	}

	if (!image.data) { // Check for invalid input
		UIPF_LOG_WARNING("unable to read image ", strFilename);
		//throw ErrorException(string("Could not open or find the image: ") + strFilename);
	}
	return image;
}

void LoadImages::run() {

	using namespace boost::filesystem;

	// TODO refactor

	List::ptr list(new List());

	std::string sPath = getParam<std::string>("path","./images");
	std::string mode = getParam<std::string>("mode", "color");

	UIPF_LOG_TRACE("Looking for images in ", sPath);

	path p(sPath);
//	if (p.is_relative()) {
//		p = system_complete(p);
//	}

	try {
		if (!exists(p)) {
			throw ErrorException(std::string("Image search path does not exist: ") + p.string());
		} else if (is_directory(p)) {

			directory_iterator end_itr;
			for (directory_iterator itr( p ); itr != end_itr; ++itr) {
				// try to load library if it is a file ending with system specific name, .so/.dll
				if (is_regular_file(itr->path())) {
					Mat mat = loadimage(itr->path().string(), mode);
					if (mat.data) {
						OpenCVMat::ptr image(new OpenCVMat(mat));
						image->filename = itr->path().string();
						list->getContent().push_back(image);
					}
				}
			}

		} else if (is_regular_file(p)) {
			Mat mat = loadimage(p.string(), mode);
			if (mat.data) {
				OpenCVMat::ptr image(new OpenCVMat(mat));
				image->filename = p.string();
				list->getContent().push_back(image);
			}
		} else {
			throw ErrorException(std::string("Image search path is not a regular file or directory: ") + p.string());
		}
	}
	catch (const filesystem_error& ex) {
		throw ErrorException(std::string("Failed to read image search path: ") + p.string() + std::string(" : ") + ex.what());
	}

	UIPF_LOG_INFO("loaded ", list->getContent().size(), " images from ", p.string());

	setOutputData<List>("imageset", list);
}
